// ALRoamingController.h
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ALRoamingController.generated.h"

UCLASS()
class AILECTURE_API AALRoamingController : public AAIController
{
	GENERATED_BODY()

public:
	AALRoamingController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	void MoveToRandomLocation();

	FTimerHandle RandomMoveTimer;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MoveRadius = 1000.0f;
};
