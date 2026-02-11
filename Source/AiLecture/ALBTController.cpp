// Fill out your copyright notice in the Description page of Project Settings.

#include "ALBTController.h"
#include "BehaviorTree/BehaviorTree.h"

AALBTController::AALBTController()
{
}

void AALBTController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}
