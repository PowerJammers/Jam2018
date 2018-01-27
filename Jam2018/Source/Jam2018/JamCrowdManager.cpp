// Fill out your copyright notice in the Description page of Project Settings.

#include "JamCrowdManager.h"
#include "MoveTowardsPosBehaviorComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AJamCrowdManager::AJamCrowdManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJamCrowdManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AJamCrowdManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<int> to_delete;
	for (int i = 0; i < MovingMembers.Num(); i++)
	{
		FCrowdGroupMember & moving_agent = MovingMembers[i];
		FVector cur_pos = moving_agent.actor->GetActorLocation();

		FVector dir = moving_agent.group->GroupMidpoint - cur_pos;
		float distance_to_target = dir.Size();
		if (distance_to_target > moving_agent.group->GroupRadius + 10.f)
		{

		}
		else
		{
			to_delete.Add(i);
			moving_agent.group->GroupMembers.Add(moving_agent);
			SetLocationsOfGroupMembers(*moving_agent.group);
			//if (auto * moveCmp = moving_agent.actor->FindComponentByClass<UMoveTowardsPosBehaviorComponent>())
			//{
			//	moveCmp->SetTargetPos(moving_agent.actor->GetActorLocation(), 10.f);
			//}
		}
	}

	int difference = 0;
	for (int index : to_delete)
	{
		MovingMembers.RemoveAt(index - difference);
		difference++;
	}


	if (MovingMembers.Num() < MaxAgentsMoving)
	{
		if (EllapsedTimeBetweenAgentsMoving > TimeBetweenAgentsMoving)
		{
			if (SetAgentToMove())
				EllapsedTimeBetweenAgentsMoving = 0.f;
		}
		else
		{
			EllapsedTimeBetweenAgentsMoving += DeltaTime;
		}
	}
}

void AJamCrowdManager::OrganizeGroups()
{
	for (AGhostCharacter * actor : CrowdActors)
	{
		bool added = false;
		FVector pos = actor->GetActorLocation();
		for (FCrowdGroup & group : CrowdGroups)
		{
			float dist2 = FVector::DistSquared(pos, group.GroupMidpoint);

			if (dist2 <= MaxGroupRadiusSq)
			{
				added = true;
				FVector midpoint = FVector::ZeroVector;
				FCrowdGroupMember new_member;
				new_member.actor = actor;
				new_member.group = &group;
				group.GroupMembers.Add(new_member);
				for (FCrowdGroupMember & member : group.GroupMembers)
				{
					midpoint += member.actor->GetActorLocation();
				}
				group.GroupMidpoint = midpoint / group.GroupMembers.Num();
				break;
			}
		}
		if (!added)
		{
			FCrowdGroup new_group;
			CrowdGroups.Add(new_group);

			FCrowdGroup & group = CrowdGroups.Last();
			FCrowdGroupMember new_member;
			new_member.actor = actor;
			new_member.group = &group;
			group.GroupMembers.Add(new_member);
			group.GroupMidpoint = pos;
		}
	}
}
void AJamCrowdManager::SetLocationsOfGroupMembers()
{
	for (FCrowdGroup & group : CrowdGroups)
	{
		SetLocationsOfGroupMembers(group);
	}
}
void AJamCrowdManager::SetLocationsOfGroupMembers(FCrowdGroup & group)
{
	float radius_per_member = 50.f;
	int group_size = group.GroupMembers.Num();
	if (group_size == 1)
		return;
	float radius = radius_per_member * group_size;
	group.GroupRadius = radius;
	float degrees_delta = 360.f / group_size;
	float degrees = 0.f;
	FVector midpoint = FVector::ZeroVector;
	for (FCrowdGroupMember & member : group.GroupMembers)
	{
		midpoint += member.actor->GetActorLocation();
	}
	group.GroupMidpoint = midpoint / group.GroupMembers.Num();

	for (FCrowdGroupMember & member : group.GroupMembers)
	{
		float x = FMath::Cos(FMath::DegreesToRadians(degrees));
		float y = FMath::Sin(FMath::DegreesToRadians(degrees));

		FVector pos_in_circle(x, y, 0);
		pos_in_circle *= radius;

		member.actor->MoveToLocation(group.GroupMidpoint + pos_in_circle, group.GroupMidpoint);
		degrees += degrees_delta;
	}
}

bool AJamCrowdManager::SetAgentToMove()
{
	int index_group_from = FMath::RandRange(0, CrowdGroups.Num() - 1);
	int index_group_to = FMath::RandRange(0, CrowdGroups.Num() - 1);

	if (index_group_from == index_group_to)
		return false;

	FCrowdGroup & group_from = CrowdGroups[index_group_from];

	if (group_from.GroupMembers.Num() == 0)
	{
		CrowdGroups.RemoveAt(index_group_from);
		return false;
	}

	if (group_from.GroupMembers.Num() <= 2)
		return false;

	int index_actor = FMath::RandRange(0, group_from.GroupMembers.Num() - 1);
	FCrowdGroup & group_to = CrowdGroups[index_group_to];

	FCrowdGroupMember new_moving;
	new_moving.actor = group_from.GroupMembers[index_actor].actor;
	new_moving.group = &group_to;

	group_from.GroupMembers.RemoveAt(index_actor);
	MovingMembers.Add(new_moving);

	SetLocationsOfGroupMembers(group_from);

	new_moving.actor->MoveToLocation(group_to.GroupMidpoint, group_to.GroupMidpoint);
	return true;
}


void AJamCrowdManager::DeleteAgent(AGhostCharacter * agent)
{
	bool found = false;
	for (FCrowdGroup & group : CrowdGroups)
	{

		for (int i = 0; i < group.GroupMembers.Num(); i++)
		{
			if (group.GroupMembers[i].actor == agent)
			{
				group.GroupMembers.RemoveAt(i);
				found = true;
				break;
			}
		}
		if (found)
		{
			break;
		}
	}

	if (!found)
	{
		for (int i = 0; i < MovingMembers.Num(); i++)
		{
			if (MovingMembers[i].actor == agent)
			{
				MovingMembers.RemoveAt(i);
				break;
			}
		}
	}

	for (int i = 0; i < CrowdActors.Num(); i++)
	{
		if (CrowdActors[i] == agent)
		{
			CrowdActors.RemoveAt(i);
			return;
		}
	}
	//agent->DestroyActor();
}