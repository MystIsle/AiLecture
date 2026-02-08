// ALAIDebugLibrary.h
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ALAIDebugLibrary.generated.h"

class AAIController;

UCLASS()
class AILECTURE_API UALAIDebugLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** AI 이동 경로를 디버그 라인으로 그립니다. */
	static void DrawFollowingPath(AAIController* Controller, float LifeTime = 2.0f);
};
