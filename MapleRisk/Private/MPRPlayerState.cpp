// Fill out your copyright notice in the Description page of Project Settings.


#include "MPRPlayerState.h"
#include "Abilities/MPRAttributeSetBase.h"
#include "Abilities/MPRAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Skills/MPRSkill.h"
#include "CharacterBase.h"

AMPRPlayerState::AMPRPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMPRAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UMPRAttributeSetBase>(TEXT("AttributeSetBase"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 100.0f;
}

void AMPRPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AMPRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPRPlayerState, PlayerSkills);
}

UAbilitySystemComponent* AMPRPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMPRAttributeSetBase* AMPRPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

void AMPRPlayerState::SetPlayerSkills(const TArray<UMPRSkill*>& InSkills)
{
	PlayerSkills = InSkills;
}

bool AMPRPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float AMPRPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float AMPRPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float AMPRPlayerState::GetMoveSpeed() const
{
	return AttributeSetBase->GetMoveSpeed();
}

int32 AMPRPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetCharacterLevel();
}

const TArray<UMPRSkill*>& AMPRPlayerState::GetPlayerSkills() const
{
	return PlayerSkills;
}
