// ALFSMController.cpp
#include "ALFSMController.h"
#include "ALAIDebugLibrary.h"
#include "ALAIStateBase.h"
#include "ALIdleState.h"
#include "ALPatrolState.h"
#include "ALChaseState.h"
#include "ALAttackState.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AALFSMController::AALFSMController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AALFSMController::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 참조 확보
	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// 상태 객체 생성
	IdleState = NewObject<UALIdleState>(this);
	IdleState->Init(this);
	PatrolState = NewObject<UALPatrolState>(this);
	PatrolState->Init(this);
	ChaseState = NewObject<UALChaseState>(this);
	ChaseState->Init(this);
	AttackState = NewObject<UALAttackState>(this);
	AttackState->Init(this);

	ChangeState(IdleState);
}

void AALFSMController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 이동 방향으로 캐릭터 회전
	if (ACharacter* PossessedCharacter = Cast<ACharacter>(InPawn))
	{
		PossessedCharacter->bUseControllerRotationYaw = false;
		if (UCharacterMovementComponent* MovComp = PossessedCharacter->GetCharacterMovement())
		{
			MovComp->bOrientRotationToMovement = true;
		}
	}
}

void AALFSMController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Switch 없음! 현재 상태 객체에게 위임
	if (CurrentState)
	{
		CurrentState->TickState(DeltaTime);
	}
}

// ========================================
// 상태 전환
// ========================================
void AALFSMController::ChangeState(UALAIStateBase* NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	if (CurrentState)
	{
		CurrentState->ExitState();
	}

	CurrentState = NewState;

	if (CurrentState)
	{
		CurrentState->EnterState();
	}
}

// ========================================
// 유틸리티
// ========================================
bool AALFSMController::IsPlayerInRange(float Range) const
{
	if (TargetPlayer == nullptr || GetPawn() == nullptr)
	{
		return false;
	}

	const float Distance = FVector::Dist(GetPawn()->GetActorLocation(), TargetPlayer->GetActorLocation());

	return Distance <= Range;
}

void AALFSMController::MoveToRandomLocation()
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
		UALAIDebugLibrary::DrawFollowingPath(this);
	}
}

void AALFSMController::MoveToPlayer()
{
	if (TargetPlayer == nullptr)
	{
		return;
	}

	MoveToActor(TargetPlayer, AttackRange);
	UALAIDebugLibrary::DrawFollowingPath(this);
}

bool AALFSMController::IsMoving() const
{
	return GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Moving;
}
