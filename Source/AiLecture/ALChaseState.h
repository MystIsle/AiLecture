// ALChaseState.h
#pragma once

#include "ALAIStateBase.h"
#include "ALChaseState.generated.h"

UCLASS()
class UALChaseState : public UALAIStateBase
{
	GENERATED_BODY()

public:
	virtual void EnterState() override;
	virtual void TickState(float DeltaTime) override;
	virtual void ExitState() override;
};
