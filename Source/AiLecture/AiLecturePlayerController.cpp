// Copyright Epic Games, Inc. All Rights Reserved.

#include "AiLecturePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AiLectureCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AISense_Hearing.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AAiLecturePlayerController::AAiLecturePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AAiLecturePlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AAiLecturePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AAiLecturePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AAiLecturePlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AAiLecturePlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AAiLecturePlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AAiLecturePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AAiLecturePlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AAiLecturePlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AAiLecturePlayerController::OnTouchReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}


void AAiLecturePlayerController::DrawFollowingPath(float LifeTime /*= 2.0f*/)
{
#if ENABLE_DRAW_DEBUG
	UPathFollowingComponent* PathComp = FindComponentByClass<UPathFollowingComponent>();
	if (!PathComp)
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
			GetWorld(),
			Points[i].Location,
			Points[i + 1].Location,
			FColor::Green,
			false,
			LifeTime,
			0,
			3.0f
		);

		DrawDebugSphere(
			GetWorld(),
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
			GetWorld(),
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

void AAiLecturePlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AAiLecturePlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();

	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AAiLecturePlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAISense_Hearing::ReportNoiseEvent(
			GetWorld(),
			CachedDestination, // 클릭한 위치
			1.0f, // Loudness
			GetPawn(),
			3000.0f // MaxRange
		);
		//DrawFollowingPath(3);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true,
		                                               ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AAiLecturePlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AAiLecturePlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}
