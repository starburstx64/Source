// Fill out your copyright notice in the Description page of Project Settings.


#include "MPRCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "CharacterBase.h"

UMPRCharacterMovementComponent::UMPRCharacterMovementComponent()
{
	SprintSpeedMultiplier = 1.4f;
	ADSSpeedMultiplier = 0.8f;
	KnockedDownSpeedMultiplier = 0.4f;
}

float UMPRCharacterMovementComponent::GetMaxSpeed() const
{
	ACharacterBase* Owner = Cast<ACharacterBase>(GetOwner());
	
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	if (!Owner->IsAlive())
	{
		return 0.0f;
	}

	if (RequestToStartSprinting)
	{
		return Owner->GetMoveSpeed() * SprintSpeedMultiplier;
	}

	if (RequestToStartADS)
	{
		return Owner->GetMoveSpeed() * ADSSpeedMultiplier;
	}

	return Owner->GetMoveSpeed();
}

void UMPRCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;

	RequestToStartADS = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}

FNetworkPredictionData_Client* UMPRCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UMPRCharacterMovementComponent* MutableThis = const_cast<UMPRCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FMPRNetworkPredictionData_Client(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UMPRCharacterMovementComponent::StartSprinting()
{
	RequestToStartSprinting = true;
}

void UMPRCharacterMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}

void UMPRCharacterMovementComponent::StartAimDownSights()
{
	RequestToStartADS = true;
}

void UMPRCharacterMovementComponent::StopAimDownSights()
{
	RequestToStartADS = false;
}

void UMPRCharacterMovementComponent::FMPRSavedMove::Clear()
{
	Super::Clear();

	SavedRequestToStartSprinting = false;
	SavedRequestToStartADS = false;
}

uint8 UMPRCharacterMovementComponent::FMPRSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}

	if (SavedRequestToStartADS)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}

bool UMPRCharacterMovementComponent::FMPRSavedMove::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedRequestToStartSprinting != ((FMPRSavedMove*)&NewMove)->SavedRequestToStartSprinting)
	{
		return false;
	}

	if (SavedRequestToStartADS != ((FMPRSavedMove*)&NewMove)->SavedRequestToStartADS)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UMPRCharacterMovementComponent::FMPRSavedMove::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UMPRCharacterMovementComponent* CharacterMovement = Cast<UMPRCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
		SavedRequestToStartADS = CharacterMovement->RequestToStartADS;
	}
}

void UMPRCharacterMovementComponent::FMPRSavedMove::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UMPRCharacterMovementComponent* CharacterMovement = Cast<UMPRCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}

UMPRCharacterMovementComponent::FMPRNetworkPredictionData_Client::FMPRNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr UMPRCharacterMovementComponent::FMPRNetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FMPRSavedMove());
}
