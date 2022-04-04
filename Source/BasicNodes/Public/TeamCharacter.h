// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "TeamCharacter.generated.h"

UCLASS(Blueprintable)
class BASICNODES_API ATeamCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATeamCharacter();
	
	/** Team ID of the actor. 255 is neutral, same ID's are friendly, all others are enemies. */
	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = FGenericTeamId::NoTeam;

	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/** struct that holds team info */
	FGenericTeamId Team;

};
