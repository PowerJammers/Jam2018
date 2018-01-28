// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "JamCrowdManager.generated.h"

struct FCrowdGroup;
class AGhostCharacter;

USTRUCT()
struct FCrowdGroupMember
{
	GENERATED_BODY()

	AGhostCharacter * actor;
	FCrowdGroup * group;
};

USTRUCT()
struct FCrowdGroup
{
	GENERATED_BODY()

	TArray<FCrowdGroupMember> GroupMembers;
	FVector GroupMidpoint;
	float GroupRadius = 0;
};

UCLASS()
class JAM2018_API AJamCrowdManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJamCrowdManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OrganizeGroups();

	UFUNCTION(BlueprintCallable)
	void SetLocationsOfGroupMembers();

	UPROPERTY(BlueprintReadWrite)
	TArray<AGhostCharacter*> CrowdActors;

	UPROPERTY(BlueprintReadWrite)
	TArray<FCrowdGroup> CrowdGroups;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxGroupRadius;
	UPROPERTY(BlueprintReadWrite)
	float MaxGroupRadiusSq;

	UPROPERTY(EditDefaultsOnly)
	float AgentsMoveVelocity;

	UPROPERTY(EditDefaultsOnly)
	int MaxAgentsMoving;

	UPROPERTY(EditDefaultsOnly)
	float TimeBetweenAgentsMoving;

	float EllapsedTimeBetweenAgentsMoving;

	private:

	TArray<FCrowdGroupMember> MovingMembers;
	void SetLocationsOfGroupMembers(FCrowdGroup & group, bool update_group_mid = false);

	bool SetAgentToMove();

	UFUNCTION(BlueprintCallable)
	void DeleteAgent(AGhostCharacter * agent);

};
