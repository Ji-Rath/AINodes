#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetSplinePosition.generated.h"

struct FBlackboardKeySelector;

UCLASS()
class SOLDIERAI_API UBTTask_GetSplinePosition : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	/** Current index on spline */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PatrolIndex;

	/** Spline Component to patrol */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PatrolSpline;

	/** The output spline position */
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector OutPosition;

	UBTTask_GetSplinePosition();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

};