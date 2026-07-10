// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner->GetCapsuleComponent()->OnComponentHit.AddUniqueDynamic(this, &UCustomCharacterMovementComponent::OnCapsuleHit);
}

void UCustomCharacterMovementComponent::OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Black, TEXT("Capsule Hit"));
	}
}
