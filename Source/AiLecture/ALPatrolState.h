// ALPatrolState.h
#pragma once

#include "ALAIStateBase.h"
#include "ALPatrolState.generated.h"

UCLASS()
class UALPatrolState : public UALAIStateBase
{
	GENERATED_BODY()

public:
	virtual void EnterState() override;
	virtual void TickState(float DeltaTime) override;
	virtual void ExitState() override;
};
