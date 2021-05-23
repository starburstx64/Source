// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPRTypes.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "MPRPlayerState.generated.h"

class UMPRAbilitySystemComponent;
class UMPRAttributeSetBase;
class UMPRSkill;

/**
 * 
 */
UCLASS()
class MAPLERISK_API AMPRPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AMPRPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UMPRAttributeSetBase* GetAttributeSetBase() const;

	void SetPlayerSkills(const TArray<UMPRSkill*>& InSkills);

	UFUNCTION(BlueprintCallable, Category = "MPRPlayerState")
	bool IsAlive() const;

	/**
	* Getters for attributes from AttributeSetBase. Returns Current Value unless otherwise specified.
	*/
	UFUNCTION(BlueprintCallable, Category = "MPRPlayerState")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "MPRPlayerState")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "MPRPlayerState")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "MPRPlayerState")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "MPRPlayerState")
	const TArray<UMPRSkill*>& GetPlayerSkills() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	UMPRAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UMPRAttributeSetBase* AttributeSetBase;

	UPROPERTY(Replicated)
	TArray<UMPRSkill*> PlayerSkills;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
