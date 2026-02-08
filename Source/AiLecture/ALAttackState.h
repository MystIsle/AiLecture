// ALAttackState.h
#pragma once

#include "ALAIStateBase.h"
#include "ALAttackState.generated.h"

UCLASS()
class UALAttackState : public UALAIStateBase
{
	GENERATED_BODY()

public:
	virtual void EnterState() override;
	virtual void TickState(float DeltaTime) override;
	virtual void ExitState() override;
};
