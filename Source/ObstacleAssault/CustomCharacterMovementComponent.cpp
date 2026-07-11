// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"
#include <GameFramework/Character.h>
#include <Components/CapsuleComponent.h>
#include "WallRunnableInterface.h"
#include "CustomMovementModes.h"

void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner->GetCapsuleComponent()->OnComponentHit.AddUniqueDynamic(this, &UCustomCharacterMovementComponent::OnCapsuleHit);
	PrevNonWallRunnableActor = nullptr;
	


}

void UCustomCharacterMovementComponent::OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == PrevNonWallRunnableActor) return;


	if (CanWallRun())
	{
		if (Cast<IWallRunnableInterface>(OtherActor))
		{
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
	return IsFalling();
}

void UCustomCharacterMovementComponent::InitWallRun()
{
	SetMovementMode(MOVE_Custom, CMOVE_WallRunning);
}
