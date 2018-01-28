// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "Characteristic.generated.h"

UCLASS()
class JAM2018_API ACharacteristic : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACharacteristic();

	int GetParameterAmount() { return mvParameters.size(); }
	virtual void SetParameter(int id, float value) {};
	void GetData(int& object, int& type) { object = mObject; type = mType; }
	int GetCorrespondingValue(float fract){return fract*mType*0.999;}

	UPROPERTY(Editanywhere)
	bool mbIsMust = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Editanywhere)
	int mObject;
	UPROPERTY(Editanywhere)
	int mType;

	std::vector<float> mvParameters; // The floats are temporal, PARAMETER class goes here
	
};