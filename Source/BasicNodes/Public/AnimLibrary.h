#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnimLibrary.generated.h"

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "AnimLibrary"))
class BASICNODES_API UAnimLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FRotator SmoothLookAt(const AActor* Actor, const FRotator CurrentRotation, const FRotator RotationTarget, const float DeltaSeconds, const float RotationLimit = 0.f, const float InterpSpeed = 3.f);
};
