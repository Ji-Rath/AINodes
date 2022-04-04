#include "TeamAIController.h"



ATeamAIController::ATeamAIController()
{
	
}

void ATeamAIController::BeginPlay()
{
	Super::BeginPlay();

	// Teams setup
	SetGenericTeamId(FGenericTeamId(TeamID));
}