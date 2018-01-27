// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characteristic.h"
#include "DummyCharacteristic.generated.h"

/**
 * 
 */
UCLASS()
class JAM2018_API ADummyCharacteristic : public ACharacteristic
{
	GENERATED_BODY()
public:
	ADummyCharacteristic() { mvParameters.push_back(0.1f); mvParameters.push_back(0.1f);
	};

	virtual void SetParameter(int id, float value) { ; }
};
