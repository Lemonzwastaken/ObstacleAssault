// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"


UENUM(BlueprintType)
enum WallRunSide : uint8
{
	EWRS_None	UMETA(DisplayName = "None"),
	EWRS_LeftSide UMETA(DisplayName = "Left Side"),
	EWRS_RightSide UMETA(DisplayName = "Right Side")
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

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> PrevNonWallRunnableActor{ nullptr };



protected:
	
	UFUNCTION()
	virtual void OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	bool CanWallRun() const;

	virtual void InitWallRun();

};
