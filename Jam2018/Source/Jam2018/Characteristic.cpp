// Fill out your copyright notice in the Description page of Project Settings.

#include "Characteristic.h"


// Sets default values
ACharacteristic::ACharacteristic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mvParameters.push_back(0.f);

}

// Called when the game starts or when spawned
void ACharacteristic::BeginPlay()
{
	Super::BeginPlay();
	
}

