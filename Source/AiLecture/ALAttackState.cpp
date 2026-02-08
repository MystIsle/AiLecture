// ALAttackState.cpp
#include "ALAttackState.h"
#include "ALFSMController.h"

void UALAttackState::EnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("[AI] Enter Attack"));
	OwnerController->StopMovement();
}

void UALAttackState::TickState(float DeltaTime)
{
	if (OwnerController->IsPlayerInRange(OwnerController->GetAttackRange()) == false)
	{
		OwnerController->ChangeState(OwnerController->GetChaseState());
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[AI] Attacking!"));
}

void UALAttackState::ExitState()
{
}
