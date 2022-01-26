#include "SoldierPerceptionComponent.h"

void USoldierPerceptionComponent::HandleExpiredStimulus(FAIStimulus& StimulusStore)
{
	Super::HandleExpiredStimulus(StimulusStore);
	
	OnExpiredStimulus.Broadcast(StimulusStore);
}
