// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"
#include <GameFramework/Character.h>
#include <Components/CapsuleComponent.h>
#include "WallRunnableInterface.h"
#include "CustomMovementModes.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Components/SplineComponent.h>
#include "GrindingPlatform.h"
#include <GameFramework/Character.h>
#include <Components/CapsuleComponent.h>

void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	bWantsToWallRun = false;

	WallSearchTraceDistance = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.0;
	CharacterOwner->GetCapsuleComponent()->OnComponentHit.AddUniqueDynamic(this, &UCustomCharacterMovementComponent::OnCapsuleHit);
	PrevNonWallRunnableActor = nullptr;
}

void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	WallRunControlInputVector = {};
}

void UCustomCharacterMovementComponent::AddInputVector(FVector WorldVector, bool bForce)
{
	if (IsWallRunning())
	{
		WallRunControlInputVector = WorldVector;

	}
	else
	{
		Super::AddInputVector(WorldVector, bForce);
	}
}

bool UCustomCharacterMovementComponent::IsWallRunning() const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == CMOVE_WallRunning;
}

bool UCustomCharacterMovementComponent::CanAttemptJump() const
{
	if (IsWallRunning())
	{
		return bWallRunInitiated;
	}

	return Super::CanAttemptJump();
}


void UCustomCharacterMovementComponent::WallRunStart()
{
	if (bAutoWallRun) return;

	bWantsToWallRun = true;
}

void UCustomCharacterMovementComponent::WallRunStop()
{
	if (bAutoWallRun) return;

	bWantsToWallRun = false;

	if (IsWallRunning())
	{
		SetMovementMode(EMovementMode::MOVE_Falling);
	}
}


void UCustomCharacterMovementComponent::OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == PrevNonWallRunnableActor) return;

	if (CanWallRun())
	{
		if (Cast<IWallRunnableInterface>(OtherActor))
		{
			WallRunHitResult = Hit;
			InitWallRun();
		}
		else
		{
			PrevNonWallRunnableActor = OtherActor;
		}
	}
}

bool UCustomCharacterMovementComponent::CanWallRun() const
{

	return (bAutoWallRun||bWantsToWallRun) && IsFalling() && !(GetWorld()->GetTimerManager().IsTimerActive(WallRunCoolDownTimer));

}

void UCustomCharacterMovementComponent::InitWallRun()
{
	WallRunControlInputVector = {};
	bWallRunInitiated = false;
	bIsTurningAroundCorner = false;

	SetMovementMode(MOVE_Custom, CMOVE_WallRunning);

	const double RightProjWallNormal = FVector::DotProduct(CharacterOwner->GetActorRightVector(), WallRunHitResult.ImpactNormal);

	WallRunSide = (RightProjWallNormal > 0.0) ? EWRS_LeftSide : EWRS_RightSide;

	FRotator TargetRotation{};
	CalcWallRunRotation(TargetRotation);

	const FLatentActionInfo LatentActionInfo{ 0, INDEX_NONE, TEXT("OnWallRunInitComplete"), this };
	static constexpr float MoveDuration = 0.2f;
	UKismetSystemLibrary::MoveComponentTo(CharacterOwner->GetRootComponent(), CharacterOwner->GetActorLocation(), TargetRotation, true, true, MoveDuration, true, EMoveComponentAction::Move, LatentActionInfo);
}

void UCustomCharacterMovementComponent::CalcWallRunRotation(FRotator& OutWallRunRotation)
{
	FVector Y{};
	if (WallRunSide == EWRS_LeftSide)
	{
		Y = WallRunHitResult.ImpactNormal;
	}
	else
	{
		Y = -WallRunHitResult.ImpactNormal;
	}

	const FVector X = FVector::CrossProduct(Y, CharacterOwner->GetActorUpVector()).GetSafeNormal();

	OutWallRunRotation = FRotationMatrix::MakeFromXY(X, Y).Rotator();
}

void UCustomCharacterMovementComponent::OnWallRunInitComplete()
{
	bWallRunInitiated = true;
}

void UCustomCharacterMovementComponent::PhysCustom(float deltatime, int32 Iterations)
{
	Super::PhysCustom(deltatime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_WallRunning:
		PhysWallRunning(deltatime, Iterations);
		break;
	default:
		break;
	}
}

void UCustomCharacterMovementComponent::PhysWallRunning(float deltatime, int32 Iterations)
{
	if (!bWallRunInitiated || bIsTurningAroundCorner || deltatime < MIN_TICK_TIME) return;


	FVector TraceStart = CharacterOwner->GetActorLocation();
	FVector TraceDirection = CharacterOwner->GetActorForwardVector();
	FVector TraceEnd = TraceStart + TraceDirection * WallSearchTraceDistance;
	GetWorld()->LineTraceSingleByChannel(WallRunHitResult, TraceStart, TraceEnd, ECC_Visibility);

	if (WallRunHitResult.bBlockingHit)
	{
		HandleWallRunCorner(ECT_Inner);
		return;
	}


	if (WallRunSide == EWRS_LeftSide)
	{
		TraceDirection = -CharacterOwner->GetActorRightVector();
	}
	else
	{
		TraceDirection = CharacterOwner->GetActorRightVector();
	}

	TraceEnd = TraceStart + TraceDirection * WallSearchTraceDistance;

	GetWorld()->LineTraceSingleByChannel(WallRunHitResult, TraceStart, TraceEnd, ECC_Visibility);

	if (WallRunHitResult.bBlockingHit)
	{
		const FVector CharacterToWall = WallRunHitResult.ImpactPoint - CharacterOwner->GetActorLocation();
		SafeMoveUpdatedComponent(CharacterToWall, CharacterOwner->GetActorRotation(), true, WallRunHitResult);

		FRotator TargetRotation{};
		CalcWallRunRotation(TargetRotation);
		const FRotator InterpedTargetRotation = FMath::RInterpTo(CharacterOwner->GetActorRotation(), TargetRotation, deltatime, WallRunRotationInterpSpeed);

		Velocity = CharacterOwner->GetActorForwardVector() * WallRunSpeed;

		const FVector AdjustedVelocity = Velocity * deltatime;
		SafeMoveUpdatedComponent(AdjustedVelocity, InterpedTargetRotation, true, WallRunHitResult);

		return;
	}

	if (WallRunSide == EWRS_LeftSide)
	{
		TraceStart = CharacterOwner->GetActorLocation() + -CharacterOwner->GetActorRightVector() * WallSearchTraceDistance;
	}
	else
	{
		TraceStart = CharacterOwner->GetActorLocation() + CharacterOwner->GetActorRightVector() * WallSearchTraceDistance;
	}

	TraceDirection = -CharacterOwner->GetActorForwardVector();
	TraceEnd = TraceStart + TraceDirection * WallSearchTraceDistance;

	GetWorld()->LineTraceSingleByChannel(WallRunHitResult, TraceStart, TraceEnd, ECC_Visibility);

	if (WallRunHitResult.bBlockingHit)
	{
		HandleWallRunCorner(ECT_Outer);
		return;
	}

	SetMovementMode(EMovementMode::MOVE_Falling);


}

void UCustomCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == EMovementMode::MOVE_Custom && PreviousCustomMode == CMOVE_WallRunning)
	{
		GetWorld()->GetTimerManager().SetTimer(WallRunCoolDownTimer, WallRunCoolDownDuration, false);
	}
}

void UCustomCharacterMovementComponent::HandleWallRunCorner(const ECornerType CornerType)
{

	FRotator TargetRotation{};
	CalcWallRunRotation(TargetRotation);

	const FVector CornerTurnDirection = FRotationMatrix(TargetRotation).GetUnitAxis(EAxis::X);
	const bool bPlayerWantsToTurn = FVector::DotProduct(CornerTurnDirection, WallRunControlInputVector) > 0.0f;

	if (bPlayerWantsToTurn)
	{
		bIsTurningAroundCorner = true;
		OnCornerTurnBegin.ExecuteIfBound(CornerTurnDirection, CornerType);

		const FVector TargetLocation = WallRunHitResult.ImpactPoint + WallRunHitResult.ImpactNormal * CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();

		const FLatentActionInfo LatentActionInfo{ 0, INDEX_NONE, TEXT("OnTurnedAroundCorner"), this };
		UKismetSystemLibrary::MoveComponentTo(CharacterOwner->GetCapsuleComponent(), TargetLocation, TargetRotation, true, true, WallRunCornerTurnDuration, true, EMoveComponentAction::Move, LatentActionInfo);
	}
	else
	{
		SetMovementMode(EMovementMode::MOVE_Falling);
	}

	const FVector TargetLocation = WallRunHitResult.ImpactPoint + WallRunHitResult.ImpactNormal * CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();

	const FLatentActionInfo LatentActionInfo{ 0, INDEX_NONE, TEXT("OnTurnedAroundCorner"), this };
	UKismetSystemLibrary::MoveComponentTo(CharacterOwner->GetCapsuleComponent(), TargetLocation, TargetRotation, true, true, WallRunCornerTurnDuration, true, EMoveComponentAction::Move, LatentActionInfo);
}

void UCustomCharacterMovementComponent::OnTurnedAroundCorner()
{
	bIsTurningAroundCorner = false;
	OnCornerTurnEnd.ExecuteIfBound();

}

void UCustomCharacterMovementComponent::PhysFalling(float deltatime, int32 Iterations)
{

	Super::PhysFalling(deltatime, Iterations);

	const bool bEnteredGrind = TryEnterGrind();

}

bool UCustomCharacterMovementComponent::TryEnterGrind()
{

	if ((MovementMode != EMovementMode::MOVE_Falling) || (Velocity.Z >= 0.0)) return false;


	FHitResult HitResult{};
	const FVector TraceStart = GetActorFeetLocation();
	const FVector TraceEnd = TraceStart + FVector{ 0.0, 0.0, -1.0 };
	constexpr ECollisionChannel GrindCollisionChannel = ECC_GameTraceChannel1;


	GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity, GrindCollisionChannel, FCollisionShape::MakeSphere(GrindDetectionRadius));



	DrawDebugSphere(GetWorld(), TraceStart, GrindDetectionRadius, 32, FColor::Red);

	if (!HitResult.bBlockingHit) return false;

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("Hit Grinding Platform"));

	GrindingPlatform = CastChecked<AGrindingPlatform>(HitResult.GetActor());
	const FVector CharacterLocation = GetActorLocation();
	const float CharacterHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	for (const TObjectPtr<USplineComponent> GrindSpline : GrindingPlatform->GetGrindSplines())
	{
		FTransform GrindTransform = GrindSpline->FindTransformClosestToWorldLocation(CharacterLocation, ESplineCoordinateSpace::World);
		const FVector CharacterHeightOffset = GrindTransform.GetUnitAxis(EAxis::Z) * CharacterHalfHeight;
		GrindTransform.AddToTranslation(CharacterHeightOffset);

		if (FVector::Distance(CharacterLocation, GrindTransform.GetLocation()) > GrindDetectionRadius)
		{
			continue;
		}

		DrawDebugSphere(GetWorld(), GrindTransform.GetLocation(), GrindDetectionRadius, 32, FColor::Blue);

	}

	return true;

}
