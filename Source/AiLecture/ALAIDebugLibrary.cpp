// ALAIDebugLibrary.cpp
#include "ALAIDebugLibrary.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "DrawDebugHelpers.h"

void UALAIDebugLibrary::DrawFollowingPath(AAIController* Controller, float LifeTime)
{
#if ENABLE_DRAW_DEBUG
	if (Controller == nullptr)
	{
		return;
	}

	UPathFollowingComponent* PathComp = Controller->GetPathFollowingComponent();
	if (PathComp == nullptr)
	{
		return;
	}

	FNavPathSharedPtr Path = PathComp->GetPath();
	if (!Path.IsValid() || !Path->IsValid())
	{
		return;
	}

	const TArray<FNavPathPoint>& Points = Path->GetPathPoints();
	for (int32 i = 0; i < Points.Num() - 1; ++i)
	{
		DrawDebugLine(
			Controller->GetWorld(),
			Points[i].Location,
			Points[i + 1].Location,
			FColor::Green,
			false,
			LifeTime,
			0,
			3.0f
		);

		DrawDebugSphere(
			Controller->GetWorld(),
			Points[i].Location,
			15.0f,
			8,
			FColor::Yellow,
			false,
			LifeTime
		);
	}

	// 최종 목적지 표시
	if (Points.Num() > 0)
	{
		DrawDebugSphere(
			Controller->GetWorld(),
			Points.Last().Location,
			25.0f,
			12,
			FColor::Red,
			false,
			LifeTime
		);
	}
#endif
}
