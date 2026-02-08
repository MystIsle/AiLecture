// ALBasicFSMController.cpp
#include "ALBasicFSMController.h"
#include "ALAIDebugLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

AALBasicFSMController::AALBasicFSMController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AALBasicFSMController::BeginPlay()
{
	Super::BeginPlay();

	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// 초기 상태 설정!
	ChangeState(EALAIState::Idle);
}

void AALBasicFSMController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AALBasicFSMController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case EALAIState::Idle:
		TickIdle(DeltaTime);
		break;
	case EALAIState::Patrol:
		TickPatrol(DeltaTime);
		break;
	case EALAIState::Chase:
		TickChase(DeltaTime);
		break;
	case EALAIState::Attack:
		TickAttack(DeltaTime);
		break;
	}
}

void AALBasicFSMController::ChangeState(EALAIState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[AI] State: %s -> %s"),
		*UEnum::GetValueAsString(CurrentState),
		*UEnum::GetValueAsString(NewState));

	// 상태 종료시 처리할 위치

	CurrentState = NewState;

	// 상태 진입 시 초기화
	switch (NewState)
	{
	case EALAIState::Patrol:
		MoveToRandomLocation();
		UALAIDebugLibrary::DrawFollowingPath(this);
		break;
	case EALAIState::Chase:
		if (TargetPlayer)
		{
			MoveToActor(TargetPlayer, AttackRange);
			UALAIDebugLibrary::DrawFollowingPath(this);
		}
		break;
	case EALAIState::Attack:
		StopMovement();
		break;
	default:
		StopMovement();
		break;
	}
}

// 상태별 행동
void AALBasicFSMController::TickIdle(float DeltaTime)
{
	if (IsPlayerInRange(SightRange))
	{
		ChangeState(EALAIState::Chase);
		return;
	}

	ChangeState(EALAIState::Patrol);
}

void AALBasicFSMController::TickPatrol(float DeltaTime)
{
	if (IsPlayerInRange(SightRange))
	{
		ChangeState(EALAIState::Chase);
		return;
	}

	if (GetPathFollowingComponent()->GetStatus() != EPathFollowingStatus::Moving)
	{
		MoveToRandomLocation();
		UALAIDebugLibrary::DrawFollowingPath(this);
	}
}

void AALBasicFSMController::TickChase(float DeltaTime)
{
	if (TargetPlayer == nullptr)
	{
		return;
	}

	if (IsPlayerInRange(AttackRange))
	{
		ChangeState(EALAIState::Attack);
		return;
	}

	if (IsPlayerInRange(SightRange) == false)
	{
		ChangeState(EALAIState::Patrol);
		return;
	}

	MoveToActor(TargetPlayer, AttackRange);
	UALAIDebugLibrary::DrawFollowingPath(this);
}

void AALBasicFSMController::TickAttack(float DeltaTime)
{
	if (IsPlayerInRange(AttackRange) == false)
	{
		ChangeState(EALAIState::Chase);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[AI] Attacking!"));
}

// --- 유틸리티 ---
bool AALBasicFSMController::IsPlayerInRange(float Range) const
{
	if (TargetPlayer == nullptr || GetPawn() == nullptr)
	{
		return false;
	}

	const float Distance = FVector::Dist(GetPawn()->GetActorLocation(), TargetPlayer->GetActorLocation());

	return Distance <= Range;
}

void AALBasicFSMController::MoveToRandomLocation()
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
	const bool bFound = NavSystem->GetRandomReachablePointInRadius(MyPawn->GetActorLocation(), MoveRadius, RandomLocation);
	if (bFound)
	{
		MoveToLocation(RandomLocation.Location);
	}
}
