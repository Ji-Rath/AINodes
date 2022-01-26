#include "BTTask_SetWalkSpeed.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTask_SetWalkSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	APawn* OwnerPawn = Controller->GetPawn();

	if (auto* CharacterMovement = Cast<UCharacterMovementComponent>(OwnerPawn->GetMovementComponent()))
	{
		CharacterMovement->MaxWalkSpeed = NewSpeed;
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
