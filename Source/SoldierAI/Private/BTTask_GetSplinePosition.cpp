
#include "BTTask_GetSplinePosition.h"
#include "Components/SplineComponent.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTTask_GetSplinePosition::UBTTask_GetSplinePosition()
{
	PatrolIndex.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetSplinePosition, PatrolIndex));
	OutPosition.AddVectorFilter(this , GET_MEMBER_NAME_CHECKED(UBTTask_GetSplinePosition, OutPosition));
	PatrolSpline.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetSplinePosition, PatrolSpline), USplineComponent::StaticClass());
	PatrolSpline.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetSplinePosition, PatrolSpline), AActor::StaticClass());

	NodeName = "GetSplinePosition";
}

EBTNodeResult::Type UBTTask_GetSplinePosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	check(Blackboard);
	
	UObject* SplineKeyValue = Blackboard->GetValueAsObject(PatrolSpline.SelectedKeyName);
	USplineComponent* Spline = Cast<USplineComponent>(SplineKeyValue);
	
	if (!Spline)
	{
		// If an actor was passed in, try to get spline component
		if (AActor* SplineActor = Cast<AActor>(SplineKeyValue))
		{
			Spline = SplineActor->FindComponentByClass<USplineComponent>();
		}
	}

	const int32 IndexValue = Blackboard->GetValueAsInt(PatrolIndex.SelectedKeyName);

	if (ensureMsgf(Spline, TEXT("Error in task %s: Cannot fetch spline component or there are no valid points!")))
	{
		int32 SplinePointCount = Spline->GetNumberOfSplinePoints();

		// Return spline point location and update patrol index value
		const FVector PatrolPosition = Spline->GetLocationAtSplinePoint(IndexValue, ESplineCoordinateSpace::World);
		Blackboard->SetValueAsVector(OutPosition.SelectedKeyName, PatrolPosition);
		Blackboard->SetValueAsInt(PatrolIndex.SelectedKeyName, IndexValue);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_GetSplinePosition::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		OutPosition.ResolveSelectedKey(*BBAsset);
		PatrolIndex.ResolveSelectedKey(*BBAsset);
		PatrolSpline.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}
