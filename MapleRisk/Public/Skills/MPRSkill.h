// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "MPRAssetManager.h"
#include "MPRTypes.h"
#include "Abilities/MPRGameplayAbility.h"
#include "MPRSkill.generated.h"

/**
 * 
 */

UCLASS()
class MAPLERISK_API UMPRSkill : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UMPRSkill()
		: NumOfCharges(0)
		, MaxLevel(1)
		, AbilityLevel(1)
	{

	}

	/** User-visible skill type */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill)
	ESkillType SkillType;

	/** User-visible short name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill)
	FText SkillName;

	/** User-visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill)
	FText SkillDescription;

	/** Icon to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill)
	FSlateBrush SkillIcon;

	/** The number of charges that this ability has <= 0 means no charges */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill)
	int32 NumOfCharges;

	/** Maximum level this skill can be, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill)
	int32 MaxLevel;

	/** Abilities to grant if this Skill is slotted */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UMPRGameplayAbility>> GrantedAbilities;

	/** Ability level this skill grants. <= 0 means the character level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	int32 AbilityLevel;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
