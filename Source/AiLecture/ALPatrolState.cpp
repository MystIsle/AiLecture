// ALPatrolState.cpp
#include "ALPatrolState.h"
#include "ALFSMController.h"

void UALPatrolState::EnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("[AI] Enter Patrol"));
	OwnerController->MoveToRandomLocation();
}

void UALPatrolState::TickState(float DeltaTime)
{
	// 플레이어 발견 -> Chase
	if (OwnerController->IsPlayerInRange(OwnerController->GetSightRange()))
	{
		OwnerController->ChangeState(OwnerController->GetChaseState());
		return;
	}

	// 도착했으면 다음 랜덤 위치
	if (OwnerController->IsMoving() == false)
	{
		OwnerController->MoveToRandomLocation();
	}
}

void UALPatrolState::ExitState()
{
	OwnerController->StopMovement();
}
