// Fill out your copyright notice in the Description page of Project Settings.


#include "MPREngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void UMPREngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UAbilitySystemGlobals::Get().InitGlobalData();

	UE_LOG(LogTemp, Warning, TEXT("Engine Subsystem"));
}