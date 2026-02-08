// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AALAIController.generated.h"

UCLASS()
class AILECTURE_API AALAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AALAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
