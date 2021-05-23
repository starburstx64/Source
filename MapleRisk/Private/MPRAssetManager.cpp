// Fill out your copyright notice in the Description page of Project Settings.


#include "MPRAssetManager.h"
#include "Skills/MPRSkill.h"
#include "AbilitySystemGlobals.h"

const FPrimaryAssetType	UMPRAssetManager::Skill = TEXT("Skill");

UMPRAssetManager& UMPRAssetManager::Get()
{
	UMPRAssetManager* This = Cast<UMPRAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		//UE_LOG(LogActionRPG, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be RPGAssetManager!"));
		return *NewObject<UMPRAssetManager>(); // never calls this
	}
}

void UMPRAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}

UMPRSkill* UMPRAssetManager::ForceLoadSkill(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UMPRSkill* LoadedSkill = Cast<UMPRSkill>(ItemPath.TryLoad());

	if (bLogWarning && LoadedSkill == nullptr)
	{
		//UE_LOG(LogActionRPG, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedSkill;
}