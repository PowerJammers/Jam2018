// Fill out your copyright notice in the Description page of Project Settings.

#include "GhostCharacter.h"


// Sets default values
AGhostCharacter::AGhostCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGhostCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGhostCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGhostCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AGhostCharacter::MoveToLocation_Implementation(FVector target_pos, FVector LookAtPosFinished)
{
	return false;
}
bool AGhostCharacter::SetCharacteristic_Implementation(const int CharID, const int TypeId)
{
	return false;
}
