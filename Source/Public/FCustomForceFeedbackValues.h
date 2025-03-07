#pragma once

#include "CoreMinimal.h"
#include "FCustomForceFeedbackValues.generated.h"

USTRUCT(BlueprintType)
struct FCustomForceFeedbackValues
{
	GENERATED_BODY() // Necessário para compilar com o UHT

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback")
	float LeftLarge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback")
	float LeftSmall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback")
	float RightLarge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Feedback")
	float RightSmall;

	FCustomForceFeedbackValues()
		: LeftLarge(0.f)
		, LeftSmall(0.f)
		, RightLarge(0.f)
		, RightSmall(0.f)
	{ }
};
