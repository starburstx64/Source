// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterBase.h"
#include "MPRPlayerState.h"
#include "Abilities/MPRGameplayAbility.h"
#include "Abilities/TargetActors/MPRGATargetActor_LineTrace.h"
#include "Skills/MPRSkill.h"

APlayerCharacterBase::APlayerCharacterBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bASCInputBound(false)
{
	
}

/**
* On the Server, Possession happens before BeginPlay.
* On the Client, BeginPlay happens before Possession.
* So we can't use BeginPlay to do anything with the AbilitySystemComponent because we don't have it until the PlayerState replicates from possession.
*/
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	AMPRPlayerState* PS = GetPlayerState<AMPRPlayerState>();

	// Client
	if (AbilitySystemComponent && PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin Play"));
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	}
}

void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	BindASCInput();
}

void APlayerCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMPRPlayerState* PS = GetPlayerState<AMPRPlayerState>();

	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UMPRAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		UE_LOG(LogTemp, Warning, TEXT("Possessed By"));

		// Set Hero Abilities in player state to display later in the HUD.
		TArray<UMPRSkill*> PlayerSkills;
		HeroAbilities.GenerateValueArray(PlayerSkills);
		PS->SetPlayerSkills(PlayerSkills);

		//// Set the AttributeSetBase for convenience attribute functions
		AttributeSetBase = PS->GetAttributeSetBase();

		if (GetLocalRole() == ROLE_Authority && !AbilitySystemComponent->bCharacterAbilitiesGiven)
		{
			for (const UMPRSkill* Skill : PlayerSkills)
			{
				const auto& GrantedAbilities = Skill->GrantedAbilities;

				for (const TSubclassOf<UMPRGameplayAbility>& Ability : GrantedAbilities)
				{
					AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
				}
			}
		}

		InitializeAttributes();
		AddStartupEffects();
		AddCharacterAbilities();
	}
}

// Client Only
void APlayerCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AMPRPlayerState* PS = GetPlayerState<AMPRPlayerState>();

	if (PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("On Rep Player State"));

		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UMPRAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		BindASCInput();

		// Set the AttributeSetBase for convenience attribute functions
		AttributeSetBase = PS->GetAttributeSetBase();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that posession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();
	}
}

AMPRGATargetActor_LineTrace* APlayerCharacterBase::GetLineTraceTargetActor()
{
	if (LineTraceTargetActor)
	{
		return LineTraceTargetActor;
	}

	LineTraceTargetActor = GetWorld()->SpawnActor<AMPRGATargetActor_LineTrace>();
	LineTraceTargetActor->SetOwner(this);

	return LineTraceTargetActor;
}

void APlayerCharacterBase::BindASCInput()
{
	if (!bASCInputBound && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EPlayerAbilityInputID"), static_cast<int32>(EPlayerAbilityInputID::ConfirmTarget), static_cast<int32>(EPlayerAbilityInputID::CancelTarget)));

		bASCInputBound = true;

		UE_LOG(LogTemp, Warning, TEXT("Bind ASC"));
	}
}
