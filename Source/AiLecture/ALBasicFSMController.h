// ALBasicFSMController.h
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ALAIState.h"
#include "ALBasicFSMController.generated.h"

UCLASS()
class AILECTURE_API AALBasicFSMController : public AAIController
{
	GENERATED_BODY()

public:
	AALBasicFSMController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	using AController::ChangeState; // 경고 제거용 코드입니다.
	
private:
	// --- FSM ---
	
	
	void ChangeState(EALAIState NewState);

	// --- 상태별 Tick ---
	void TickIdle(float DeltaTime);
	void TickPatrol(float DeltaTime);
	void TickChase(float DeltaTime);
	void TickAttack(float DeltaTime);
	
	// --- 유틸리티 ---
	bool IsPlayerInRange(float Range) const;
	void MoveToRandomLocation();
	
	UPROPERTY()
	APawn* TargetPlayer = nullptr;
	
	EALAIState CurrentState = EALAIState::Idle;
	
	float SightRange = 500.0f;
	float AttackRange = 100.0f;
	float MoveRadius = 800.0f;
};
