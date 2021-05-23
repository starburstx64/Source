// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPRTypes.h"
#include "CharacterBase.h"
#include "Skills/MPRSkill.h"
#include "PlayerCharacterBase.generated.h"

class UMPRGameplayAbility;
class AMPRGATargetActor_LineTrace;

/**
 * 
 */
UCLASS()
class MAPLERISK_API APlayerCharacterBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerCharacterBase(const class FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	/* Bind ASC */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	// Client Only
	virtual void OnRep_PlayerState() override;

	// Getter for LineTraceTargetActor. Spawns it if it doesn't exist yet. Only for characters that have a ranged weapon
	UFUNCTION(BlueprintCallable, Category = "Abilities|Targeting")
	AMPRGATargetActor_LineTrace* GetLineTraceTargetActor();

protected:
	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();

	bool bASCInputBound;

	// Assets, Probablemente deba refactorizar esto
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TMap<ESkillType, UMPRSkill*> HeroAbilities;

	UPROPERTY()
	AMPRGATargetActor_LineTrace* LineTraceTargetActor;
};
