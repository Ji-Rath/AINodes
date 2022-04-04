// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamCharacter.h"

// Sets default values
ATeamCharacter::ATeamCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ATeamCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetGenericTeamId(FGenericTeamId(TeamID));
}

// Called every frame
void ATeamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATeamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATeamCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	Team = NewTeamID;
}

FGenericTeamId ATeamCharacter::GetGenericTeamId() const
{
	return Team;
}

