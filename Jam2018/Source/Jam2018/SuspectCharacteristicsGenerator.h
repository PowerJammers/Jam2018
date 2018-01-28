// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characteristic.h"
#include "GhostCharacter.h"
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
	TArray<int> char_order;
	bool dead = false;
};

UCLASS()
class JAM2018_API ASuspectCharacteristicsGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuspectCharacteristicsGenerator();

	UFUNCTION(BlueprintCallable)
	bool IsSuspectObj(int id);

	UFUNCTION(BlueprintCallable)
	bool IsSuspectHinter(int id);

	UFUNCTION(BlueprintCallable)
	void SuspectDied(int id);

	void AddCharacteristic(ACharacteristic* characteristic);

	UFUNCTION(BlueprintNativeEvent)
	bool RegisterCrowd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateSuspects();
	void DistributeCharacteristics();
	void DistributeParameters();
	void ModifyMeshes();
	void GetObjectiveTypes();

	bool AreHintersLeft();
	void GetNewHinters();
	bool IsTypeObjectives(int charact, int type);

	UPROPERTY(Editanywhere)
	TSubclassOf<AGhostCharacter> mGhostCharacter[3];
private:

	void output(int c, int t);

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
	std::vector<AGhostCharacter *> mvpCharacters;

	int obj_char, obj_type, hint_char, hint_type;
	std::vector<int> mvObjectiveParams;
	bool obj_set = false;
};
