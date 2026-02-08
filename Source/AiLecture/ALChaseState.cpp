// ALChaseState.cpp
#include "ALChaseState.h"
#include "ALFSMController.h"

void UALChaseState::EnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("[AI] Enter Chase"));
	OwnerController->MoveToPlayer();
}

void UALChaseState::TickState(float DeltaTime)
{
	// 공격 범위 안 -> Attack
	if (OwnerController->IsPlayerInRange(OwnerController->GetAttackRange()))
	{
		OwnerController->ChangeState(OwnerController->GetAttackState());
		return;
	}

	// 시야 밖 -> Patrol로 복귀
	if (OwnerController->IsPlayerInRange(OwnerController->GetSightRange()) == false)
	{
		OwnerController->ChangeState(OwnerController->GetPatrolState());
		return;
	}

	// 계속 추적
	OwnerController->MoveToPlayer();
}

void UALChaseState::ExitState()
{
	OwnerController->StopMovement();
}
