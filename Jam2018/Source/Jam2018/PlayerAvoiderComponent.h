// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAvoiderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable )
class JAM2018_API UPlayerAvoiderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAvoiderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetPlayer(AActor * player);
	
	AActor * Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DistanceToAvoidPlayer;

	UPROPERTY(BlueprintReadWrite)
	float Distance2ToAvoidPlayer;
};
