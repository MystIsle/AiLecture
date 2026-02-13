// Fill out your copyright notice in the Description page of Project Settings.

#include "ALBTController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"


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
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		// 눈으로 봤다 → 직접 추적
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Actor);
            
			// 적을 직접 봤으므로 소리 조사 취소
			GetBlackboardComponent()->ClearValue(TEXT("HearingLocation"));
		}
		else
		{
			GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
		}
		return;
	}
    
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		// 소리를 들었다 → 소리 위치로 조사
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsVector(
				TEXT("HearingLocation"), Stimulus.StimulusLocation);
		}
		return;
	}
}
