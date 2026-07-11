#pragma once
#include "CustomMovementModes.generated.h"


UENUM(BlueprintType)
enum ECustomWallMovementMode : int
{
	CMOVE_WallRunning  UMETA(DisplayName = "WallRunning"),

	CMOVE_MAX		   UMETA(Hidden),
};
