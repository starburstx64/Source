// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "AICharacterBase.generated.h"

class UMPRAbilitySystemComponent;
class UMPRAttributeSetBase;
class UMPRFloatingStatusBarWidget;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class MAPLERISK_API AAICharacterBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AAICharacterBase(const class FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<UMPRFloatingStatusBarWidget> UIFloatingStatusBarClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	UWidgetComponent* UIFloatingStatusBarComponent;

	UPROPERTY()
	UMPRFloatingStatusBarWidget* UIFloatingStatusBar;

	FDelegateHandle HealthChangedDelegateHandle;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
};
