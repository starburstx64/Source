// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/MPRDamageExecutionCalculation.h"
#include "Abilities/MPRAttributeSetBase.h"
#include "Abilities/MPRAbilitySystemComponent.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct MPRDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalAttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BonusDamagePerLevel);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalAttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDamageReduction);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalDamageReduction);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	MPRDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// Capture the Source's AttackPower. We do want to snapshot this at the moment we create the GameplayEffectSpec that will execute the damage.
		// (imagine we fire a projectile: we create the GE Spec when the projectile is fired. When it hits the target, we want to use the AttackPower at the moment
		// the projectile was launched, not when it hits).
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMPRAttributeSetBase, PhysicalAttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMPRAttributeSetBase, MagicalAttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMPRAttributeSetBase, BonusDamagePerLevel, Source, true);

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMPRAttributeSetBase, Damage, Source, true);

		// Capture the Target's Defences. Don't snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMPRAttributeSetBase, PhysicalDamageReduction, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMPRAttributeSetBase, MagicalDamageReduction, Target, false);
	}
};

static const MPRDamageStatics& DamageStatics()
{
	static MPRDamageStatics DStatics;
	return DStatics;
}

UMPRDamageExecutionCalculation::UMPRDamageExecutionCalculation()
{
	HeadShotMultiplier = 1.5f;

	RelevantAttributesToCapture.Add(DamageStatics().PhysicalAttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicalAttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().BonusDamagePerLevelDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalDamageReductionDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicalDamageReductionDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UMPRDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayTagContainer AssetTags;
	Spec.GetAllAssetTags(AssetTags);

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Target defences
	float PhysicalDamageReduction = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().PhysicalDamageReductionDef, EvaluationParameters, PhysicalDamageReduction);
	PhysicalDamageReduction = FMath::Max<float>(PhysicalDamageReduction, 0.0f);

	float MagicalDamageReduction = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicalDamageReductionDef, EvaluationParameters, MagicalDamageReduction);
	MagicalDamageReduction = FMath::Max<float>(MagicalDamageReduction, 0.0f);

	// Get Source Attack power
	float PhysicalAttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().PhysicalAttackPowerDef, EvaluationParameters, PhysicalAttackPower);
	PhysicalAttackPower = FMath::Max<float>(PhysicalAttackPower, 0.0f);

	float MagicalAttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicalAttackPowerDef, EvaluationParameters, MagicalAttackPower);
	MagicalAttackPower = FMath::Max<float>(MagicalAttackPower, 0.0f);

	float BonusDamagePerLevel = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BonusDamagePerLevelDef, EvaluationParameters, BonusDamagePerLevel);
	BonusDamagePerLevel = FMath::Max<float>(BonusDamagePerLevel, 0.0f);

	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	float RawDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, RawDamage);
	
	// Get SetByCaller damage if it exists
	float RawPhysicalDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.PhysicalDamage")), false, -1.0f), 0.0f);
	float RawMagicalDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.MagicalDamage")), false, -1.0f), 0.0f);

	// Check for headshot(Physical).
	const FHitResult* Hit = Spec.GetContext().GetHitResult();
	if (AssetTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.CanHeadShot"))) && Hit && Hit->BoneName == "b_head")
	{
		RawPhysicalDamage *= HeadShotMultiplier;
		FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();
		MutableSpec->DynamicAssetTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.HeadShot")));
	}

	float UnmitigatedPhysicalDamage = (RawPhysicalDamage + BonusDamagePerLevel) * PhysicalAttackPower;
	float MitigatedPhysicalDamage = FMath::Max<float>(UnmitigatedPhysicalDamage - (UnmitigatedPhysicalDamage * PhysicalDamageReduction), 0.0f);

	float UnmitigatedMagicalDamage = (RawMagicalDamage + BonusDamagePerLevel) * MagicalAttackPower;
	float MitigatedMagicalDamage = FMath::Max<float>(UnmitigatedMagicalDamage - (UnmitigatedMagicalDamage * MagicalDamageReduction), 0.0f);

	float DamageDone = MitigatedPhysicalDamage + MitigatedMagicalDamage + RawDamage;
	UE_LOG(LogTemp, Warning, TEXT("Damage done: %f"), DamageDone);

	if (DamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, DamageDone));
	}
}
