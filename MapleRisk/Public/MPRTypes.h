// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/PrimaryAssetId.h"
#include "MPRTypes.generated.h"

// ----------------------------------------------------------------------------------------------------------------
// This header is for enums and structs used by classes and blueprints accross the game
// Collecting these in a single header helps avoid problems with recursive header includes
// It's also a good place to put things like data table row structs
// ----------------------------------------------------------------------------------------------------------------

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	NoType		UMETA(DisplayName = "No Type"),			// Nunca se debe usar
	Basic		UMETA(DisplayName = "Basic Skill"),
	Heavy		UMETA(DisplayName = "Heavy Skill"),
	Special1	UMETA(DisplayName = "Special Skill 1"),
	Special2	UMETA(DisplayName = "Special Skill 2"),
	Ultimate	UMETA(DisplayName = "Ultimate Skill"),
};


UENUM(BlueprintType)
enum class EPlayerAbilityInputID : uint8
{
	None			UMETA(DisplayName = "None"),
	ConfirmTarget	UMETA(DisplayName = "Confirm Target"),
	CancelTarget	UMETA(DisplayName = "Cancel Target"),
	Jump			UMETA(DisplayName = "Jump"),
	Sprint			UMETA(DisplayName = "Sprint"),
	Interact		UMETA(DisplayName = "Interact"),
	BasicSkill		UMETA(DisplayName = "Use Basic Skill"),
	HeavySkill		UMETA(DisplayName = "Use Heavy Skill"),
	SpecialSkill1	UMETA(DisplayName = "Use Special Skill 1"),
	SpecialSkill2	UMETA(DisplayName = "Use Special Skill 2"),
	Ultimate		UMETA(DisplayName = "Use Ultimate Skill"),
};

