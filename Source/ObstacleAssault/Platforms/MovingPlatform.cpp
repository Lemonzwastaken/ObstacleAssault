// Fill out your copyright notice in the Description page of Project Settings.


#include "Platforms/MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void MyTestFunction(float MyFloatParam, FString MyStringParam)
{
	
	UE_LOG(LogTemp, Display, TEXT("My float param is %f"), MyFloatParam);
	UE_LOG(LogTemp, Display, TEXT("My Text param is %s"), *MyStringParam);


}



// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();


	FString MyName = GetName();

	StartLocation = GetActorLocation();

	MyTestFunction(3.5f, MyName);

}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);

}

//Platform Movement
void AMovingPlatform::MovePlatform(float DeltaTime)
{
	// Moves the platform by a set velocity in Delta Time
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation = CurrentLocation + (PlatformVelocity * DeltaTime);
	SetActorLocation(CurrentLocation);

	DistanceMoved = FVector::Dist(StartLocation, CurrentLocation);

}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	//Rotates platform by delta time
}