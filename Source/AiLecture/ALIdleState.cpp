// ALIdleState.cpp
#include "ALIdleState.h"
#include "ALFSMController.h"

void UALIdleState::EnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("[AI] Enter Idle"));
	OwnerController->StopMovement();
}

void UALIdleState::TickState(float DeltaTime)
{
	// 플레이어 발견 -> Chase
	if (OwnerController->IsPlayerInRange(OwnerController->GetSightRange()))
	{
		OwnerController->ChangeState(OwnerController->GetChaseState());
		return;
	}

	// 가만히 있다가 순찰로 전환
	OwnerController->ChangeState(OwnerController->GetPatrolState());
}

void UALIdleState::ExitState()
{
}
