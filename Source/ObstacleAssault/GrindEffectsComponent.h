// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrindEffectsComponent.generated.h"

class UNiagraComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OBSTACLEASSAULT_API UGrindEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrindEffectsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(EditAnywhere, Category = "GrindEffects", meta = (DisplayName = "Left Foot Grind Sparks"))
	TObjectPtr<UNiagraComponent>LeftFootGrindSparks;

	UPROPERTY(EditAnywhere, Category = "GrindEffects", meta = (DisplayName = "Right Foot Grind Sparks"))
	TObjectPtr<UNiagraComponent>LeftFootGrindSparks;




};
