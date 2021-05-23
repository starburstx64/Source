// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/MPRSkill.h"

FString UMPRSkill::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UMPRSkill::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix
	return FPrimaryAssetId("Skill", GetFName()); // FUCK!!! El primer argumento debe ser igual con PrimaryAssetType.
}
