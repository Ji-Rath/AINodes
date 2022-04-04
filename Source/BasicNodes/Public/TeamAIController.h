#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TeamAIController.generated.h"

UCLASS(Blueprintable)
class BASICNODES_API ATeamAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	/** Team ID of the actor. 255 is neutral, same ID's are friendly, all others are enemies.*/
	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = FGenericTeamId::NoTeam;

	ATeamAIController();

protected:
	virtual void BeginPlay() override;
};


