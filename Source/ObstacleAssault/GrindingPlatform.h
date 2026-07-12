// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GrindingPlatform.generated.h"

class USplineComponent;


/**
 * 
 */
UCLASS()
class OBSTACLEASSAULT_API AGrindingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditInstanceOnly, meta = (DisplayName = "Grind Splines"))
	TArray<TObjectPtr<USplineComponent>> GrindSpline{};
	


};
