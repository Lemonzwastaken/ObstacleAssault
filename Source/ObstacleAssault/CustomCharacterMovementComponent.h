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


private:

	bool BWallRunInitiated = false;

	FHitResult WallRunHitResult{};

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> PrevNonWallRunnableActor{ nullptr };

	EWallRunSide WallRunSide{ EWRS_None };

protected:
	
	UFUNCTION()
	virtual void OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	bool CanWallRun() const;

	virtual void InitWallRun();

	virtual void CalcWallRunRotation(FRotator& OutWallRunRotation);

	UFUNCTION()
	virtual void OnWallRunInitComplete();


};
