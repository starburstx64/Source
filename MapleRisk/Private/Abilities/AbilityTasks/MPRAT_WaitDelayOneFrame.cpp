// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityTasks/MPRAT_WaitDelayOneFrame.h"
#include "TimerManager.h"

UMPRAT_WaitDelayOneFrame::UMPRAT_WaitDelayOneFrame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMPRAT_WaitDelayOneFrame::Activate()
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UMPRAT_WaitDelayOneFrame::OnDelayFinish);
}

UMPRAT_WaitDelayOneFrame* UMPRAT_WaitDelayOneFrame::WaitDelayOneFrame(UGameplayAbility* OwningAbility)
{
	UMPRAT_WaitDelayOneFrame* MyObj = NewAbilityTask<UMPRAT_WaitDelayOneFrame>(OwningAbility);
	return MyObj;
}

void UMPRAT_WaitDelayOneFrame::OnDelayFinish()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnFinish.Broadcast();
	}
	EndTask();
}
