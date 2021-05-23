// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MPRAssetManager.generated.h"

class UMPRSkill;

/**
 * 
 */
UCLASS()
class MAPLERISK_API UMPRAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UMPRAssetManager() {}

	virtual void StartInitialLoading() override;

	/** Static type for Assets */
	static const FPrimaryAssetType Skill;

	/** Returns the current AssetManager object */
	static UMPRAssetManager& Get();

	/**
	 * Synchronously loads an MPRSkill subclass, this can hitch but is useful when you cannot wait for an async load
	 * This does not maintain a reference to the item so it will garbage collect if not loaded some other way
	 *
	 * @param PrimaryAssetId The asset identifier to load
	 * @param bDisplayWarning If true, this will log a warning if the item failed to load
	 */
	UMPRSkill* ForceLoadSkill(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
	
};
