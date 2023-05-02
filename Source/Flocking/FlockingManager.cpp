#include "FlockingManager.h"

#define AGENT_COUNT 10

void UFlockingManager::Init( UWorld *world, UStaticMeshComponent *mesh ) {
    UE_LOG(LogTemp, Warning, TEXT("Manager initialized"));
    
    World = world;
    float incr = (PI * 2.f) / AGENT_COUNT;
    for( int i = 0; i < AGENT_COUNT; i++ ) {
        if( World != nullptr ) {
            FRotator rotation = FRotator();

            FVector location = FVector();
            location.X = FMath::Sin( incr * i ) * 550.f;
            location.Y = FMath::Sin(incr * i) * 500.f;
            location.Z = FMath::Cos( incr * i ) * 550.f;

            AAgent * agent = World->SpawnActor<AAgent>( location, rotation );
            agent->Init( mesh, i );
            Agents.Add( agent );
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {
    FVector v1, v2, v3, v4;
    for (AAgent* curAgent : Agents) {
        v1 = rule1(curAgent);
        v2 = rule2(curAgent);
        v3 = rule3(curAgent);
        v4 = wind();

        curAgent->Velocity = curAgent->Velocity + v1 + v2 + v3 + v4;
        limitVelocity(curAgent);
    }
}

FVector UFlockingManager::rule1(AAgent* agent) {
    FVector perceivedCenter;
    for (AAgent* curAgent : Agents) {
        if (curAgent != agent) {
            if ((curAgent->GetActorLocation() - agent->GetActorLocation()).Size() < 250) {
                perceivedCenter = perceivedCenter + curAgent->GetActorLocation();
            } 
        }
    }

    perceivedCenter = perceivedCenter / (Agents.Num() - 1);
    return (perceivedCenter - agent->GetActorLocation()) / 100;
}

FVector UFlockingManager::rule2(AAgent* agent) {
    FVector c = FVector(0);
    for (AAgent* curAgent : Agents) {
        if (curAgent != agent) {
            if ((curAgent->GetActorLocation() - agent->GetActorLocation()).Size() < 250) {
                c = c - ((curAgent->GetActorLocation() - agent->GetActorLocation()) / (curAgent->GetActorLocation() - agent->GetActorLocation()).Size());
            }
        }
    }
    return c;
}

FVector UFlockingManager::rule3(AAgent* agent) {
    FVector perceivedVelocity = FVector(0);

    for (AAgent* curAgent : Agents) {
        if (curAgent != agent) {
            perceivedVelocity = perceivedVelocity + curAgent->Velocity;
        }
    }

    perceivedVelocity = perceivedVelocity / (Agents.Num() - 1);
    return (perceivedVelocity - agent->Velocity) / 16;
}

FVector UFlockingManager::wind() {
    return FVector(0.05, 0, 0);
}

void UFlockingManager::limitVelocity(AAgent* agent) {
    float vlim = 50.0;
    FVector v;

    if (agent->Velocity.Size() > vlim) {
        agent->Velocity = (agent->Velocity / agent->Velocity.Size()) * vlim;
    }
}