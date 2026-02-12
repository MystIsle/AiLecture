// Fill out your copyright notice in the Description page of Project Settings.

#include "ALBTController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AALBTController::AALBTController()
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}
void AALBTController::BeginPlay()
{
	Super::BeginPlay();

	if (AIPerceptionComp)
	{
		AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(
			this, &AALBTController::OnTargetPerceptionUpdated);
	}
}

void AALBTController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

void AALBTController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		// 감지 성공 → Blackboard에 타겟 저장
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
	}
	else
	{
		// 감지 이탈 → 타겟 해제
		BlackboardComp->ClearValue(TEXT("TargetActor"));
	}
}
