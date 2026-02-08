// ALIdleState.h
#pragma once

#include "ALAIStateBase.h"
#include "ALIdleState.generated.h"

UCLASS()
class UALIdleState : public UALAIStateBase
{
	GENERATED_BODY()

public:
	virtual void EnterState() override;
	virtual void TickState(float DeltaTime) override;
	virtual void ExitState() override;
};
