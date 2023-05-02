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
            location.X = FMath::Sin( incr * i ) * 150.f;
            location.Z = FMath::Cos( incr * i ) * 150.f;

            AAgent * agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);
            Agents.Add(agent);
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
        v4 = bound_position(curAgent);

        curAgent->Velocity = curAgent->Velocity + v1 + v2 + v3 + v4;
        limitVelocity(curAgent);
    }
}

FVector UFlockingManager::rule1(AAgent* agent) {
    FVector perceivedCenter = FVector();
    for (AAgent* curAgent : Agents) {
        if (curAgent != agent) {
            perceivedCenter = perceivedCenter + curAgent->GetActorLocation();
        }
    }

    perceivedCenter = perceivedCenter / (Agents.Num() - 1);
    return (perceivedCenter - agent->GetActorLocation()) / 100;
}

FVector UFlockingManager::rule2(AAgent* agent) {
    FVector c = FVector();
    for (AAgent* curAgent : Agents) {
        if (curAgent != agent) {
            if ((curAgent->GetActorLocation() - agent->GetActorLocation()).GetAbs().Size() < 300) {
                c = c - (curAgent->GetActorLocation() - agent->GetActorLocation());
            }
        }
    }
    return c;
}

FVector UFlockingManager::rule3(AAgent* agent) {
    FVector perceivedVelocity = FVector();

    for (AAgent* curAgent : Agents) {
        if (curAgent != agent) {
            perceivedVelocity = perceivedVelocity + curAgent->Velocity;
        }
    }

    perceivedVelocity = perceivedVelocity / (Agents.Num() - 1);
    return (perceivedVelocity - agent->Velocity) / 8;
}

void UFlockingManager::limitVelocity(AAgent* agent) {
    float vlim = 75.0;
    FVector v;

    if (agent->Velocity.GetAbs().Size() > vlim) {
        agent->Velocity = (agent->Velocity / agent->Velocity.GetAbs().Size()) * vlim;
    }
}

FVector UFlockingManager::bound_position(AAgent* agent) {
    int32 Xmin = -5000;
    int32 Xmax = 5000;
    int32 Ymin = -5000;
    int32 Ymax = 5000;
    int32 Zmin = 0;
    int32 Zmax = 5000;
    FVector v = FVector();
    if (agent->GetActorLocation().X < Xmin) {
        v.X = 10;
    } else if (agent->GetActorLocation().X > Xmax) {
        v.X = -10;
    }

    if (agent->GetActorLocation().Y < Ymin) {
        v.Y = 10;
    } else if (agent->GetActorLocation().Y > Ymax) {
        v.Y = -10;
    }

    if (agent->GetActorLocation().Z < Zmin) {
        v.Z = 10;
    } else if (agent->GetActorLocation().Z > Zmax) {
        v.Z = -10;
    }
    
    return v;
}