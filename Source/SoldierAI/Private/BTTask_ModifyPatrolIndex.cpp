#include "BTTask_ModifyPatrolIndex.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SplineComponent.h"

UBTTask_ModifyPatrolIndex::UBTTask_ModifyPatrolIndex()
{
	Index.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ModifyPatrolIndex, Index));
	PatrolSpline.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ModifyPatrolIndex, PatrolSpline), USplineComponent::StaticClass());
	IncrementDirection.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ModifyPatrolIndex, IncrementDirection));
}

EBTNodeResult::Type UBTTask_ModifyPatrolIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	// Fetch blackboard values
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	UObject* SplineObject = Blackboard->GetValueAsObject(PatrolSpline.SelectedKeyName);
	USplineComponent* SplineComp = Cast<USplineComponent>(SplineObject);
	int32 SplineLength = SplineComp->GetNumberOfSplinePoints();
	SplineLength = SplineLength == 0 ? 1 : SplineLength;

	int32 IndexValue = Blackboard->GetValueAsInt(Index.SelectedKeyName);
	int32 IncrementDirectionValue = Blackboard->GetValueAsInt(IncrementDirection.SelectedKeyName);

	// Perform modifications to index
	switch (ModifyIndex)
	{
		case Reset:
			IndexValue += 1;
			if (IndexValue > SplineLength)
				IndexValue = 0;
			break;
		case Flipflop:
			IndexValue += 1 * IncrementDirectionValue;
			if (IndexValue > SplineLength || IndexValue <= 0)
				IncrementDirectionValue *= -1;
			break;
		case Random:
			IndexValue = FMath::RandRange(0, SplineLength - 1);
			break;
	}

	// Update blackboard values
	Blackboard->SetValueAsInt(Index.SelectedKeyName, IndexValue);
	Blackboard->SetValueAsInt(IncrementDirection.SelectedKeyName, IncrementDirectionValue);

	return EBTNodeResult::Succeeded;
}

void UBTTask_ModifyPatrolIndex::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		Index.ResolveSelectedKey(*BBAsset);
		PatrolSpline.ResolveSelectedKey(*BBAsset);

	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}
