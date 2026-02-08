// Fill out your copyright notice in the Description page of Project Settings.


#include "AALAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AALAIController::AALAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AALAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AALAIController::OnPossess(APawn* InPawn)
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

// Called every frame
void AALAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	// 플레이어 위치 가져오기
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	FVector MyLocation = ControlledPawn->GetActorLocation();

	// 매 프레임마다 플레이어 방향으로 직선 이동
	FVector Direction = (PlayerLocation - MyLocation).GetSafeNormal();
	ControlledPawn->AddMovementInput(Direction, 1.0f);
}

