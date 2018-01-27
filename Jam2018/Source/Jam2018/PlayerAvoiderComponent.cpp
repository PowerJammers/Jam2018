// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAvoiderComponent.h"


// Sets default values for this component's properties
UPlayerAvoiderComponent::UPlayerAvoiderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerAvoiderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerAvoiderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if(!Player)
	return;
	FVector player_pos = Player->GetActorLocation();

	FVector pos = GetOwner()->GetActorLocation();

	FVector dir = pos - player_pos;

	float distance2 = dir.SizeSquared();

	if (distance2 < Distance2ToAvoidPlayer)
	{
		dir.Normalize();
		GetOwner()->SetActorLocation(pos + dir * 400 * DeltaTime);
	}

}

void UPlayerAvoiderComponent::SetPlayer(AActor * player)
{
	Player = player;
}