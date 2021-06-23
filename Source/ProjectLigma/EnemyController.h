#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

struct FTimerHandle;

UENUM(BlueprintType)
enum EEnemyState
{
	Idle,
	Searching,
	Pursuing
};

UCLASS()
class PROJECTLIGMA_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	FName BBSearchVector;

	UPROPERTY(EditAnywhere)
	FName BBEnemyState;

	UPROPERTY(EditAnywhere)
	FName BBPlayerTarget;

	UPROPERTY(EditAnywhere)
	float DetectPlayerDelay = 1.f;

	bool bSeePlayer = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTimerHandle PursueSeenPlayerHandle;

	FTimerHandle SearchLocationHandle;

protected:

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	UFUNCTION()
	void PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void SetEnemyState(EEnemyState EnemyState);

	UFUNCTION()
	void ExpiredStimulus(const FAIStimulus& StimulusStore);

private:

	AEnemyController();

	FAISenseID SightID;
	FAISenseID HearingID;
};
