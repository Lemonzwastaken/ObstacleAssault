// Fill out your copyright notice in the Description page of Project Settings.


#include "GrindingPlatform.h"
#include <Components/SplineComponent.h>

#ifdef WITH_EDITOR
void AGrindingPlatform::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent)
{
	Super::PostEditChangeProperty(PropertyChangeEvent);

	if (PropertyChangeEvent.ChangeType == EPropertyChangeType::ArrayAdd)
	{
		const int32 NullElementIdx = GrindSplines.Find(nullptr);

		if (NullElementIdx != INDEX_NONE)
		{
			TObjectPtr<USplineComponent> NewSplineComp = Cast<USplineComponent>(AddComponentByClass(USplineComponent::StaticClass(), false, FTransform{}, false));
			if (!NewSplineComp) return;
			AddInstanceComponent(NewSplineComp);
			NewSplineComp->SetWorldScale3D(FVector{ 1.0 });
			NewSplineComp->SetAbsolute(false, false, true);
			GrindSplines[NullElementIdx] = NewSplineComp;

		}
	}


}
#endif //WITH EDITOR