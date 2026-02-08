// ALFSMController.h
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ALFSMController.generated.h"

class UALAIStateBase;

UCLASS()
class AILECTURE_API AALFSMController : public AAIController
{
	GENERATED_BODY()

public:
	AALFSMController();
	virtual void Tick(float DeltaTime) override;

	// --- FSM ---
	void ChangeState(UALAIStateBase* NewState);

	// --- State Getter ---
	UALAIStateBase* GetIdleState() const { return IdleState; }
	UALAIStateBase* GetPatrolState() const { return PatrolState; }
	UALAIStateBase* GetChaseState() const { return ChaseState; }
	UALAIStateBase* GetAttackState() const { return AttackState; }

	// --- 유틸리티 (State에서 접근) ---
	bool IsPlayerInRange(float Range) const;
	void MoveToRandomLocation();
	void MoveToPlayer();
	bool IsMoving() const;

	float GetSightRange() const { return SightRange; }
	float GetAttackRange() const { return AttackRange; }

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	using AController::ChangeState; // 경고 제거용 코드입니다.

private:
	UPROPERTY()
	UALAIStateBase* CurrentState = nullptr;

	// 상태 객체들
	UPROPERTY()
	UALAIStateBase* IdleState;
	UPROPERTY()
	UALAIStateBase* PatrolState;
	UPROPERTY()
	UALAIStateBase* ChaseState;
	UPROPERTY()
	UALAIStateBase* AttackState;

	UPROPERTY()
	APawn* TargetPlayer = nullptr;

	float SightRange = 1000.0f;
	float AttackRange = 200.0f;
	float MoveRadius = 1500.0f;
};
