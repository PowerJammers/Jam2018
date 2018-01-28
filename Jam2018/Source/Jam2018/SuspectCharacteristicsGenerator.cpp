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

bool ASuspectCharacteristicsGenerator::IsSuspectObj(int id)
{
	if (mvSuspects[id].characteristics[obj_char].mbPresent && mvSuspects[id].characteristics[obj_char].pCharacteristics->GetCorrespondingValue(mvSuspects[id].characteristics[obj_char].mvParameters[0]) == obj_type)
		return true;
	return false;
}

bool ASuspectCharacteristicsGenerator::IsSuspectHinter(int id)
{
	if (mvSuspects[id].characteristics[hint_char].mbPresent && mvSuspects[id].characteristics[hint_char].pCharacteristics->GetCorrespondingValue(mvSuspects[id].characteristics[hint_char].mvParameters[0]) == hint_type)
		return true;
	return false;
}

void ASuspectCharacteristicsGenerator::SuspectDied(int id)
{
	mvSuspects[id].dead = true;

	float a = 1;
	if (IsSuspectObj(id))
		a++; // Lose game?
	if (IsSuspectHinter(id))
	{
		if (!AreHintersLeft())
			GetNewHinters();
	}
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
	}

	CreateSuspects();

	DistributeCharacteristics();

	DistributeParameters();

	GetObjectiveTypes();

	GetNewHinters();

	ModifyMeshes();
}
void ASuspectCharacteristicsGenerator::ModifyMeshes()
{
	for (size_t i = 0; i < mSuspectAmount; ++i)
	{
		for (size_t j = 0; j < mvSuspects[i].characteristics.size(); ++j)
		{
			if (mvSuspects[i].characteristics[j].mbPresent)
			{				
				int object, type;
				mvSuspects[i].characteristics[j].pCharacteristics->GetData(object, type);
				int id = FPlatformMath::FloorToInt(mvSuspects[i].characteristics[j].mvParameters[0] * type * 0.9999f);
}

void ASuspectCharacteristicsGenerator::CreateSuspects()
{
	Suspect suspect;
	suspect.characteristics = mvCharacteristics;
	suspect.musteristics = mvMusteristics;
	for (int i = 0; i < mSuspectAmount; i++)
	{
		int randChar = FMath::RandRange(0,2);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform transform(FVector(i*100, i*100, 300.0f));
		mvpCharacters.push_back(GetWorld()->SpawnActor<AGhostCharacter>(mGhostCharacter[randChar], transform));				
		suspect.id = i;
		mvSuspects.push_back(suspect);
	}

	obj_char = FMath::RandRange(0, mvCharacteristics.size() - 1);
	obj_type = FMath::RandRange(0, mvCharacteristics[obj_char].pCharacteristics->GetCorrespondingValue(1.f));
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
					if (i == obj_char)
					{
						int type = mvSuspects[w].characteristics[i].pCharacteristics->GetCorrespondingValue(rand_buffer[id]);
						while (type == obj_type)
						{
							if (obj_set == false)
							{
								obj_set = true;
								break;
							}
							rand_buffer[id] = FMath::RandRange(0.f, 1.f);
							type = mvSuspects[w].characteristics[i].pCharacteristics->GetCorrespondingValue(rand_buffer[id]);
						}
					}

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

void ASuspectCharacteristicsGenerator::ModifyMeshes()
{
	for (size_t i = 0; i < mSuspectAmount; ++i)
	{
		for (size_t j = 0; j < mvSuspects[i].characteristics.size(); ++j)
		{
			if (mvSuspects[i].characteristics[j].mbPresent)
			{
				int object, type;
				mvSuspects[i].characteristics[j].pCharacteristics->GetData(object, type);
				int id = FPlatformMath::FloorToInt(mvSuspects[i].characteristics[j].mvParameters[0] * type - 0.001f);

				mvpCharacters[i]->SetCharacteristic(object, id);
			}
		}
	}
}

void  ASuspectCharacteristicsGenerator::GetObjectiveTypes()
{
	for (auto& it : mvSuspects)
	{
		if (it.characteristics[obj_char].mbPresent)
		{
			if (it.characteristics[obj_char].pCharacteristics->GetCorrespondingValue(it.characteristics[obj_char].mvParameters[0]) == obj_type)
			{
				for (int i = 0; i < mvCharacteristics.size(); i++)
				{
					if (it.characteristics[obj_char].mbPresent)
						mvObjectiveParams.push_back(it.characteristics[i].pCharacteristics->GetCorrespondingValue(it.characteristics[i].mvParameters[0]));
					else
						mvObjectiveParams.push_back(-1);
				}
			}
		}
	}
}

bool ASuspectCharacteristicsGenerator::AreHintersLeft()
{
	bool hinters_left = false;
	for (int i = 0; i < mvSuspects.size(); i++)
	{
		Suspect& suspect = mvSuspects[i];
		if (suspect.dead)
			continue;

		CharacteristicHolder& holder = suspect.characteristics[hint_char];
		if (holder.mbPresent)
			if (holder.pCharacteristics->GetCorrespondingValue(holder.mvParameters[0]) == hint_type)
				return true;
	}
	return false;
}

void ASuspectCharacteristicsGenerator::GetNewHinters()
{
	// Random suspect start
	int start = FMath::RandRange(0, mvSuspects.size() - 1);
	int curr = start;
	do
	{
		start++;
		if (start >= mvSuspects.size())
			start = 0;

		// Random characteristic start
		
		int start2 = FMath::RandRange(0, mvCharacteristics.size() - 1);
		int curr2 = start2;
		do
		{
			start2++;
			if (start2 >= mvCharacteristics.size())
				start2 = 0;

			// Ignore if characteristic not present
			if (mvSuspects[curr].characteristics[curr2].mbPresent == false)
				continue;

			// Random ranges inside a characteristic
			CharacteristicHolder& holder = mvSuspects[curr].characteristics[curr2];
			int max = holder.pCharacteristics->GetCorrespondingValue(1.f);
			int start3 = FMath::RandRange(0, max);
			int curr3 = start3;
			
			do
			{
				if (curr3 > max)
					curr3 = 0;

				// Check if this type is valid
				if (IsTypeObjectives(curr2, curr3) == false)
				{
					hint_char = curr2;
					hint_type = curr3;
				}

			} while (curr3 != start3);
		} while (curr2 != start2);

	} while (curr != start);
}

bool ASuspectCharacteristicsGenerator::IsTypeObjectives(int charact, int type)
{
	if (mvObjectiveParams[charact] == type)
		return true;
	return false;
}