// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GhostCharacter.generated.h"



UCLASS(ClassGroup = (GhostCharacter), meta = (BlueprintSpawnableComponent), abstract, Blueprintable)
class JAM2018_API AGhostCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGhostCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "GhostCharacter")
	void SetCharacteristic(const int CharID, const int TypeId);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GhostCharacter")
		bool MoveToLocation(FVector target_pos, FVector LookAtPosFinished);

	UPROPERTY(BlueprintReadOnly)
	int Id;
};
