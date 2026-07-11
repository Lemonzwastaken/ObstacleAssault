// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"


UENUM(BlueprintType)
enum EWallRunSide : uint8
{
	EWRS_None	UMETA(DisplayName = "None"),
	EWRS_LeftSide UMETA(DisplayName = "Left Side"),
	EWRS_RightSide UMETA(DisplayName = "Right Side"),

	EWRS_MAX	   UMETA(Hidden),
};


/**
 * 
 */
UCLASS()
class OBSTACLEASSAULT_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	virtual void AddInputVector(FVector WorldVector, bool bForce = false) override;

	bool IsWallRunning() const;

	virtual bool CanAttemptJump() const override;

private:

	bool BWallRunInitiated = false;

	FHitResult WallRunHitResult{};

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> PrevNonWallRunnableActor{ nullptr };

	FTimerHandle WallRunCoolDownTimer;


	EWallRunSide WallRunSide{ EWRS_None };

	double WallSearchTraceDistance = 0.0f;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (DisplayName = "Wall Run Interpolation Speed"))
	float WallRunRotationInterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (DisplayName = "Wall Run Speed"))
	float WallRunSpeed = 550.0f;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (DisplayName = "Wall Run Cooldown Duration"))
	float WallRunCoolDownDuration = 0.7f;


protected:
	
	UFUNCTION()
	virtual void OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	bool CanWallRun() const;

	virtual void InitWallRun();

	virtual void CalcWallRunRotation(FRotator& OutWallRunRotation);

	UFUNCTION()
	virtual void OnWallRunInitComplete();

	virtual void PhysCustom(float deltatime, int32 Iterations) override;

	virtual void PhysWallRunning(float deltatime, int32 Iterations);

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

};
