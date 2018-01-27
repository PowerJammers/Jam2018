// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveTowardsPosBehaviorComponent.h"


// Sets default values for this component's properties
UMoveTowardsPosBehaviorComponent::UMoveTowardsPosBehaviorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetComponentTickEnabled(false);
	// ...
}


// Called when the game starts
void UMoveTowardsPosBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UMoveTowardsPosBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor * owner = GetOwner();
	FVector pos = owner->GetActorLocation();

	FVector dir = Target - pos;
	float distance_to_target = dir.Size();
	if (distance_to_target > DistanceThreshold)
	{
		dir.Normalize();
		dir *= MoveVelocity*DeltaTime;

		owner->SetActorLocation(pos + dir);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s arrived to target pos"), *owner->GetName());
		SetComponentTickEnabled(false);
	}

	// ...
}
void UMoveTowardsPosBehaviorComponent::SetTargetPos(FVector target_pos, float threshold)
{
	//PrimaryComponentTick.bCanEverTick = true;
	//SetActive(true);
	SetComponentTickEnabled(true);
	Target = target_pos;
	DistanceThreshold = threshold;
}

