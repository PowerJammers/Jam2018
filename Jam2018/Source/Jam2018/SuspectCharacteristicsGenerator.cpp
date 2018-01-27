// Fill out your copyright notice in the Description page of Project Settings.

#include "SuspectCharacteristicsGenerator.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include <random>
#include <algorithm>
#include <chrono>


// Sets default values
ASuspectCharacteristicsGenerator::ASuspectCharacteristicsGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASuspectCharacteristicsGenerator::AddCharacteristic(ACharacteristic* characteristic)
{
	CharacteristicHolder holder;
	holder.pCharacteristics = characteristic;
	unsigned amount = characteristic->GetParameterAmount();
	for (unsigned i = 0; i < amount; i++)
		holder.mvParameters.push_back(-1.f); // Initialize as unset

	if (characteristic->mbIsMust)
	{
		holder.mbPresent = true;
		mvMusteristics.push_back(holder);
	}
	else
		mvCharacteristics.push_back(holder);
}

// Called when the game starts or when spawned
void ASuspectCharacteristicsGenerator::BeginPlay()
{
	Super::BeginPlay();

	// Get all of the characteristics
	TArray<AActor*> characteristics;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacteristic::StaticClass(), characteristics);
	
	// Register them
	for (int i = 0; i < characteristics.Num(); i++)
	{			
		AddCharacteristic(Cast<ACharacteristic>(characteristics[i]));
		AddCharacteristic(Cast<ACharacteristic>(characteristics[i]));
		AddCharacteristic(Cast<ACharacteristic>(characteristics[i]));
		AddCharacteristic(Cast<ACharacteristic>(characteristics[i]));
		AddCharacteristic(Cast<ACharacteristic>(characteristics[i]));
		AddCharacteristic(Cast<ACharacteristic>(characteristics[i]));
		AddCharacteristic(Cast<ACharacteristic>(characteristics[i]));
	}

	CreateSuspects();

	DistributeCharacteristics();

	DistributeParameters();
}

void ASuspectCharacteristicsGenerator::CreateSuspects()
{
	Suspect suspect;
	suspect.characteristics = mvCharacteristics;
	suspect.musteristics = mvMusteristics;
	for (int i = 0; i < mSuspectAmount; i++)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		mvpCharacters.push_back(GetWorld()->SpawnActor<AJam2018Character>(AJam2018Character::StaticClass(), SpawnInfo));
		suspect.id = i;
		mvSuspects.push_back(suspect);
	}
}

template<typename T>
int GetRandomValueFromVector(std::vector<T> values, T& value)
{
	int id = FMath::RandRange(0, values.size() - 1);
	value = values[id];
	return id;
}

void ASuspectCharacteristicsGenerator::DistributeCharacteristics()
{
	if (mvCharacteristics.size() == 0)
		return;

	int total_c = mvSuspects.size() * mMaxCharacteristicsPerSuspect;
	int amount_per_c = total_c / mvCharacteristics.size();

	// Get ready the characteristics to distribute
	std::vector<int> c_pool;
	for (int i = 0; i < mvCharacteristics.size(); i++)
	{
		int c_amount = FMath::RandRange(amount_per_c - mRandUniformityBreaker, amount_per_c + mRandUniformityBreaker);
		mvCharacteristicAmounts.push_back(c_amount);
		for (int j = 0; j < c_amount; j++)
			c_pool.push_back(i);
	}
	while (total_c > c_pool.size())
		c_pool.push_back(FMath::RandRange(0, mvCharacteristics.size() - 1));

	// Distribute them
	for(int i = 0; i < mMaxCharacteristicsPerSuspect; i++)
		for (int j = 0; j < mvSuspects.size(); j++)
		{
			int value;
			const int amount_of_tries = 3;
			for (unsigned w = 0; w < amount_of_tries; w++)
			{
				int id = GetRandomValueFromVector<int>(c_pool, value);

				// Add characteristic if not present
				if (mvSuspects[j].characteristics[value].mbPresent == false)
				{
					mvSuspects[j].characteristics[value].mbPresent = true;
					c_pool.erase(c_pool.begin() + id);
					break;
				}
			}
		}
}

void ASuspectCharacteristicsGenerator::DistributeParameters()
{
	// NORMAL CHARACTERISTICS
	for (int i = 0; i < mvCharacteristics.size(); i++)
	{
		// Fill a vector with the values for the characteristics parameters
		int amount = mvCharacteristicAmounts[i];
		float value = 0.f;
		float step = 1.f / (amount - 1);
		std::vector<float> rand_buffer;
		for (int j = 0; j < amount; j++)
		{
			rand_buffer.push_back(value);
			value += step;
		}
		
		// Randomize it and assign it to each parameter
		int param_amount = mvCharacteristics[i].pCharacteristics->GetParameterAmount();
		for (int j = 0; j < param_amount; j++)
		{
			// Randomize the buffer
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::shuffle(rand_buffer.begin(), rand_buffer.end(), std::default_random_engine(seed));

			// Assign a value to each characteristic
			int id = 0;
			for (int w = 0; w < mvSuspects.size(); w++)
			{
				CharacteristicHolder& holder = mvSuspects[w].characteristics[i];
				if (holder.mbPresent)
				{
					holder.mvParameters[j] = rand_buffer[id];
					id++;
				}
			}
		}
	}

	// MUSTERISTICS
	for (int i = 0; i < mvMusteristics.size(); i++)
	{
		// Fill a vector with the values for the characteristics parameters
		float value = 0.f;
		float step = 1.f / (mvSuspects.size() - 1);
		std::vector<float> rand_buffer;
		for (int j = 0; j < mvSuspects.size(); j++)
		{
			rand_buffer.push_back(value);
			value += step;
		}

		// Randomize it and assign it to each parameter
		int param_amount = mvMusteristics[i].pCharacteristics->GetParameterAmount();
		for (int j = 0; j < param_amount; j++)
		{
			// Randomize the buffer
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::shuffle(rand_buffer.begin(), rand_buffer.end(), std::default_random_engine(seed));

			// Assign a value to each characteristic
			for (int w = 0; w < mvSuspects.size(); w++)
			{
				CharacteristicHolder& holder = mvSuspects[w].musteristics[i];
				if (holder.mbPresent)
					holder.mvParameters[j] = rand_buffer[w];
			}
		}
	}
}

