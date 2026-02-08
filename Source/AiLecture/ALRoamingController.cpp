// ALRoamingController.cpp
#include "ALRoamingController.h"
#include "ALAIDebugLibrary.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AALRoamingController::AALRoamingController()
{
}

void AALRoamingController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AI] Controller is now controlling: %s"), *InPawn->GetName());
	}

	// 이동 방향으로 캐릭터 회전
	if (ACharacter* PossessedCharacter = Cast<ACharacter>(InPawn))
	{
		PossessedCharacter->bUseControllerRotationYaw = false;
		if (UCharacterMovementComponent* MovComp = PossessedCharacter->GetCharacterMovement())
		{
			MovComp->bOrientRotationToMovement = true;
		}
	}

	// 3초마다 랜덤 위치로 이동 (1초 후 시작)
	GetWorldTimerManager().SetTimer(
		RandomMoveTimer,
		this,
		&AALRoamingController::MoveToRandomLocation,
		3.0f,
		true,
		1.0f
	);
}

void AALRoamingController::MoveToRandomLocation()
{
	const APawn* MyPawn = GetPawn();
	if (MyPawn == nullptr)
	{
		return;
	}

	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSystem == nullptr)
	{
		return;
	}

	FNavLocation RandomLocation;
	bool bFound = NavSystem->GetRandomReachablePointInRadius(
		MyPawn->GetActorLocation(),
		MoveRadius,
		RandomLocation
	);

	if (bFound)
	{
		MoveToLocation(RandomLocation.Location);
		UALAIDebugLibrary::DrawFollowingPath(this);
	}
}
