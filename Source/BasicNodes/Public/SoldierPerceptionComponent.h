
#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "SoldierPerceptionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExpiredStimulus, const FAIStimulus&, StimulusStore);

UCLASS(ClassGroup = AI, HideCategories = (Activation, Collision), meta = (BlueprintSpawnableComponent), config = Game)
class BASICNODES_API USoldierPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FExpiredStimulus OnExpiredStimulus;

protected:
	void HandleExpiredStimulus(FAIStimulus& StimulusStore) override;

};
