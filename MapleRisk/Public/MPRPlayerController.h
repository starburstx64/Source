// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPRPlayerController.generated.h"

class UMPRHUDWidget;

/**
 * 
 */
UCLASS()
class MAPLERISK_API AMPRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void CreateHUDWidget();

	UMPRHUDWidget* GetHUDWidget();

protected:
	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<UMPRHUDWidget> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UMPRHUDWidget* UIHUDWidget;
};
