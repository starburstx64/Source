// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MPRAttributeSetBase.generated.h"

/**
 * 
 */

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class MAPLERISK_API UMPRAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMPRAttributeSetBase();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Current Health, when 0 we expect owner to die. Capped by MaxHealth */
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, Health)

	/** MaxHealth is its own attribute, since GameplayEffects may modify it */
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, MaxHealth)

	/** Bonus damage of the character according to his level. Value is 0 on Level 1*/
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_BonusDamagePerLevel)
	FGameplayAttributeData BonusDamagePerLevel;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, BonusDamagePerLevel)

	/** PhysicalAttackPower of the attacker is multiplied by the base Damage to reduce health, so 1.0 means no bonus */
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_PhysicalAttackPower)
	FGameplayAttributeData PhysicalAttackPower;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, PhysicalAttackPower)

	/** Percentage of physical damage reduction */
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_PhysicalDamageReduction)
	FGameplayAttributeData PhysicalDamageReduction;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, PhysicalDamageReduction)

	/** MagicPower of the attacker is multiplied by the base Damage to reduce health, so 1.0 means no bonus */
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_MagicalAttackPower)
	FGameplayAttributeData MagicalAttackPower;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, MagicalAttackPower)

	/** Percentage of magical damage reduction */
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_MagicalDamageReduction)
	FGameplayAttributeData MagicalDamageReduction;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, MagicalDamageReduction)

	/** MoveSpeed affects how fast characters can move */
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, MoveSpeed)

	/** Attack Speed */
	UPROPERTY(BlueprintReadOnly, Category = "CastingSpeed", ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, AttackSpeed)

	/** Casting Speed */
	UPROPERTY(BlueprintReadOnly, Category = "CastingSpeed", ReplicatedUsing = OnRep_CastingSpeed)
	FGameplayAttributeData CastingSpeed;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, CastingSpeed)

	/** Character Level */
	UPROPERTY(BlueprintReadOnly, Category = "CharacterLevel", ReplicatedUsing = OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, CharacterLevel)

	/** Damage is a 'temporary' attribute used by the DamageExecution to calculate final damage, which then turns into -Health */
	// Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "Damage", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UMPRAttributeSetBase, Damage)

protected:
	/** Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes. (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before) */
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	// These OnRep functions exist to make sure that the ability system internal representations are synchronized properly during replication
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_BonusDamagePerLevel(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_PhysicalAttackPower(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_PhysicalDamageReduction(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MagicalAttackPower(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MagicalDamageReduction(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CastingSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);
	
};
