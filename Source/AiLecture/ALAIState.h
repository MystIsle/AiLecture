// ALAIState.h
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EALAIState : uint8
{
	Idle,
	Patrol,
	Chase,
	Attack
};
