
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "SoldierPerceptionComponent.h"
#include "LightSenseComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"


void AEnemyController::IncrementAlertness(float Amount)
{
	Alertness = FMath::Clamp(Alertness + Amount, 0.f, 100.f);

	if (Blackboard && Alertness <= 0.f)
	{
		SetEnemyState(EEnemyState::Idle);
		Blackboard->SetValueAsObject(BBPlayerTarget, nullptr);
		bSeePlayer = false;
	}

	if (Alertness > 0.f)
	{
		//Pursue player if enemy is within alert range
		if (Amount > 0.f)
		{
			if (IsAlert())
			{
				SetEnemyState(EEnemyState::Pursuing);
			}

			if (GetWorldTimerManager().TimerExists(LoseInterestHandle))
			{
				// Lose Interest timer is canceled if the player is visible
				GetWorldTimerManager().ClearTimer(LoseInterestHandle);
			}

			// Search location if just patrolling
			if (GetEnemyState() == EEnemyState::Idle)
			{
				SetEnemyState(EEnemyState::Searching);
			}
		}
		else
		{
			SetEnemyState(EEnemyState::Searching);
			bSeePlayer = false;
		}
	}
	
}

bool AEnemyController::IsTargetVisible(FVector Target)
{
	bool bVisible = false;

	if (GetPawn() && bSeePlayer)
	{
		TArray<TSubclassOf<AActor>> LightActors = { AActor::StaticClass() };
		TArray<FHitResult> Hit;
		float LightLevel = ULightSenseComponent::CalculateLightLevel(GetWorld(), Target, LightActors, Hit, {});
		float Distance = FVector::Distance(GetPawn()->GetActorLocation(), Target);
		float AlertRange = UKismetMathLibrary::NormalizeToRange(Alertness, 0.f, 100.f) * (1000.f - NearsightRange);
		bVisible = (LightLevel > MinLightLevel || Distance < (AlertRange + NearsightRange));
		
	}
	
	return bVisible;
}

bool AEnemyController::IsAlert()
{
	return Alertness >= AlertnessDetectPlayer;
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	SightID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	HearingID = UAISense::GetSenseID(UAISense_Hearing::StaticClass());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::PerceptionUpdated);

	if (auto* SoldierPerception = Cast<USoldierPerceptionComponent>(PerceptionComponent))
	{
		SoldierPerception->OnExpiredStimulus.AddDynamic(this, &AEnemyController::ExpiredStimulus);
	}
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float AlertRange = UKismetMathLibrary::NormalizeToRange(Alertness, 0.f, 100.f) * (1000.f-NearsightRange);
	//DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), AlertRange + NearsightRange, 10, FColor::Red, false, 0.5f);

	if (Blackboard)
	{
		AActor* Player = Cast<AActor>(Blackboard->GetValueAsObject(BBPlayerTarget));
		if (Player)
		{
			if (IsTargetVisible(Player->GetActorLocation()))
			{
				TArray<TSubclassOf<AActor>> LightActors = { AActor::StaticClass() };
				// Calculate alertness based on target distance and light level
				float Distance = FVector::Distance(Player->GetActorLocation(), GetPawn()->GetActorLocation());
				float DistanceMultiplier = (1 - UKismetMathLibrary::NormalizeToRange(Distance, 0.f, 2000.f)) * 5.f;
				TArray<FHitResult> Hit;
				float LightLevel = ULightSenseComponent::CalculateLightLevel(GetWorld(), Player->GetActorLocation(), LightActors, Hit, {});
				float AlertIncrement = (AlertMultiplier * DistanceMultiplier) * DeltaTime;

				IncrementAlertness(AlertIncrement);
			}
			else if (!GetWorldTimerManager().TimerExists(LoseInterestHandle))
			{
				// Have a small delay before alertness begins to subtract
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUFunction(this, FName("IncrementAlertness"), DisinterestValue);
				GetWorldTimerManager().SetTimer(LoseInterestHandle, TimerDelegate, 1.f, true, 10.f);
				IncrementAlertness(-1.f);

				// Input data to blackboard to be computed in BT
				Blackboard->SetValueAsVector(BBDestinationVector, Player->GetActorLocation());
				Blackboard->SetValueAsVector(BBSearchVector, Player->GetActorLocation());
			}
		}
		else if (!GetWorldTimerManager().TimerExists(LoseInterestHandle))
		{
			// Have a small delay before alertness begins to subtract
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, FName("IncrementAlertness"), DisinterestValue);
			GetWorldTimerManager().SetTimer(LoseInterestHandle, TimerDelegate, 1.f, true, 10.f);
			IncrementAlertness(-1.f);
		}
	}
}

void AEnemyController::PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Blackboard) { return; }

	const FAISenseID SenseID = Stimulus.Type;
	AActor* CurrentTarget = Cast<AActor>(Blackboard->GetValueAsObject(BBPlayerTarget));
	bool bIsSameTarget = Actor == CurrentTarget;

	// Sight stimulus
	if (SenseID == SightID)
	{
		// Dont try to chase another person while already on another guy just because you see them LOL
		if (bSeePlayer && !bIsSameTarget) { return; }

		bSeePlayer = Stimulus.WasSuccessfullySensed();
		AActor* TargetActor = Actor;

		if (!bSeePlayer)
		{
			// If the current target is no longer seen, target another perceived actor if possible
			for (TMap<TObjectKey<AActor>, FActorPerceptionInfo>::TConstIterator StimActor = PerceptionComponent->GetPerceptualDataConstIterator(); StimActor; ++StimActor)
			{
				FActorPerceptionInfo PerceptionInfo = StimActor->Value;
				for (FAIStimulus Stim : PerceptionInfo.LastSensedStimuli)
				{
					// Ensure stimulus is sight and currently seen
					if (Stim.Type == SightID && Stim.WasSuccessfullySensed())
					{
						TargetActor = Cast<AActor>(StimActor->Key.ResolveObjectPtr());
						bSeePlayer = true;
					}
				}
			}
		}

		Blackboard->SetValueAsObject(BBPlayerTarget, TargetActor);
	}

	// Hearing stimulus
	if (SenseID == HearingID)
	{
		// Accept hearing stimulus only when the player position is unknown, and ignore if hearing stimulus expired
		if (!bSeePlayer && !Stimulus.IsExpired())
		{
			// Make controller alert on initial sound stimulus from an actor
			if (!bIsSameTarget)
			{
				IncrementAlertness(5);
				//Blackboard->SetValueAsObject(BBPlayerTarget, Actor);
			}

			// Input data to blackboard to be computed in BT
			Blackboard->SetValueAsVector(BBDestinationVector, Stimulus.StimulusLocation);
			Blackboard->SetValueAsVector(BBSearchVector, Stimulus.StimulusLocation);
		}
	}
}

void AEnemyController::SetEnemyState(EEnemyState EnemyState)
{
	Blackboard->SetValueAsEnum(BBEnemyState, (uint8) EnemyState);
}

EEnemyState AEnemyController::GetEnemyState()
{
	EEnemyState State = EEnemyState::Idle;
	if (Blackboard)
	{
		State = (EEnemyState) Blackboard->GetValueAsEnum(BBEnemyState);
	}
	return State;
}

void AEnemyController::ExpiredStimulus(const FAIStimulus& StimulusStore)
{
	// Lose interest if sight stimulus expires
	if (StimulusStore.Type == SightID)
	{
		if (!GetWorldTimerManager().TimerExists(LoseInterestHandle))
		{
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, FName("IncrementAlertness"), DisinterestValue);
			GetWorldTimerManager().SetTimer(LoseInterestHandle, TimerDelegate, 1.f, true);
			IncrementAlertness(-1.f);
		}
	}
}

AEnemyController::AEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5f;

	SetGenericTeamId(FGenericTeamId(TeamNumber));
}
