// Fill out your copyright notice in the Description page of Project Settings.


#include "MPRPlayerController.h"
#include "MPRPlayerState.h"
#include "UI/MPRHUDWidget.h"
#include "Abilities/MPRAbilitySystemComponent.h"

void AMPRPlayerController::CreateHUDWidget()
{
	if (UIHUDWidget)
	{
		return;
	}

	if (!UIHUDWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	// Only create a HUD for local player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState
	AMPRPlayerState* PS = GetPlayerState<AMPRPlayerState>();
	if (!PS)
	{
		return;
	}

	UIHUDWidget = CreateWidget<UMPRHUDWidget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();

}

UMPRHUDWidget* AMPRPlayerController::GetHUDWidget()
{
	return UIHUDWidget;
}

void AMPRPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMPRPlayerState* PS = GetPlayerState<AMPRPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void AMPRPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Hero is possessed.
	CreateHUDWidget();
}
