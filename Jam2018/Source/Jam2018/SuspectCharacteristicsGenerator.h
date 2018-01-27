// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characteristic.h"
#include <vector>
#include "SuspectCharacteristicsGenerator.generated.h"

struct CharacteristicHolder
{
	ACharacteristic* pCharacteristics;
	std::vector<float> mvParameters;
	bool mbPresent = false;
};

struct Suspect
{
	int id;
	std::vector<CharacteristicHolder> characteristics;
	std::vector<CharacteristicHolder> musteristics;
};

UCLASS()
class JAM2018_API ASuspectCharacteristicsGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuspectCharacteristicsGenerator();

	void AddCharacteristic(ACharacteristic* characteristic);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateSuspects();
	void DistributeCharacteristics();
	void DistributeParameters();

private:

	UPROPERTY(Editanywhere)
	int mSuspectAmount;
	UPROPERTY(Editanywhere)
	int mMaxCharacteristicsPerSuspect;
	UPROPERTY(Editanywhere)
	int mRandUniformityBreaker = 3;

	std::vector<CharacteristicHolder> mvCharacteristics;
	std::vector<int> mvCharacteristicAmounts;
	std::vector<CharacteristicHolder> mvMusteristics;
	std::vector<Suspect> mvSuspects;
	
};
