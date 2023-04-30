#pragma once

#include "CoreMinimal.h"
#include "Agent.h"
#include "FlockingManager.generated.h"

UCLASS()
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

	void Init( UWorld *world, UStaticMeshComponent *mesh );
	void Flock();
	FVector rule1(AAgent* agent);
	FVector rule2(AAgent* agent);
	FVector rule3(AAgent* agent);
	FVector wind();
	void limitVelocity(AAgent* agent);

private:
	UWorld *World;	
	bool initialized;
	TArray<class AAgent *> Agents;
};