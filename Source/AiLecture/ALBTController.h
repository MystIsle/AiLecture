// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ALBTController.generated.h"

class UAIPerceptionComponent;

UCLASS()
class AILECTURE_API AALBTController : public AAIController
{
	GENERATED_BODY()

public:
	AALBTController();

protected:
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
};
