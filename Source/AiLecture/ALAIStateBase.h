// ALAIStateBase.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ALAIStateBase.generated.h"

class AALFSMController;

UCLASS(Abstract)
class UALAIStateBase : public UObject
{
	GENERATED_BODY()

public:
	void Init(AALFSMController* InController) { OwnerController = InController; }

	// 상태 진입 시 호출
	virtual void EnterState() {}
	// 매 프레임 호출
	virtual void TickState(float DeltaTime) {}
	// 상태 탈출 시 호출
	virtual void ExitState() {}

protected:
	UPROPERTY()
	AALFSMController* OwnerController = nullptr;
};
