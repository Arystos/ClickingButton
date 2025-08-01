// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreeBodyProblemSubsystem.h"

#include "EngineUtils.h"
#include "ClickingButtonGame/ThreeBodyProblem/Body.h"

#pragma region FTickableGameObjectInterface
UWorld* UThreeBodyProblemSubsystem::GetTickableGameObjectWorld() const
{
	return GetWorld();
}

bool UThreeBodyProblemSubsystem::IsTickableInEditor() const
{
	return false;
}

bool UThreeBodyProblemSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UThreeBodyProblemSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UThreeBodyProblemSubsystem, STATGROUP_Tickables);
}

bool UThreeBodyProblemSubsystem::IsAllowedToTick() const
{
	return true;
}

bool UThreeBodyProblemSubsystem::IsTickable() const
{
	return true;
}

void UThreeBodyProblemSubsystem::Tick(float DeltaTime)
{
	if (Bodies.Num() == 0)
	{
		return;
	}
	
	ApplyForces(DeltaTime);
	for (ABody* Body : Bodies)
	{
		if (Body)
			Body->UpdatePosition(DeltaTime);
	}

	UE_LOG(LogTemp, Warning, TEXT("Num Bodies: %d"), Bodies.Num());
}


ETickableTickType UThreeBodyProblemSubsystem::GetTickableTickType() const
{
	return ETickableTickType::Always;
}

#pragma endregion

void UThreeBodyProblemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("Initialized SUBSYSTEM"));
	
	//FillArrayOfBodies(); // Find all bodies in the level
	
}

TArray<ABody*> UThreeBodyProblemSubsystem::FillArrayOfBodies()
{
	// Find all the body actors in the level and add them to the Bodies array
	Bodies.Empty();
	for (TActorIterator<ABody> It(GetWorld()); It; ++It)
	{
		Bodies.Add(*It);
	}
	UE_LOG(LogTemp, Warning, TEXT("Initialized subsystem %d bodies"), Bodies.Num());
	// return the array of bodies
	return Bodies;
}

TArray<ABody*> UThreeBodyProblemSubsystem::RecalculateBodies()
{
	Bodies.Empty();
	FillArrayOfBodies();
	return Bodies;
}

void UThreeBodyProblemSubsystem::CalculateGravitationalForces(ABody* A, ABody* B)
{
	if (!A || !B) return;

	FVector Dir = B->GetActorLocation() - A->GetActorLocation(); // Direction from A to B
	float Dist = FMath::Max(Dir.Size(), 1.0f); // Distance between A and B, avoid division by zero
	FVector Norm = Dir / Dist; // Normalize the direction vector

	// softened inverse-square law
	float ForceMag = FriendshipConstant * A->Mass * B->Mass
				   / (FMath::Square(Dist) + FMath::Square(Softening));
	FVector Force = Norm * ForceMag;

	A->ApplyForce( Force); 
	B->ApplyForce(-Force);

	// Apply damping if the distance exceeds the maximum allowed distance
	if (Dist > MaxDistance)
	{
		float Overshoot = Dist - MaxDistance;
		FVector Damping = Norm * FMath::Pow(Overshoot, FriendshipConstant);
		A->ApplyForce( Damping);
		B->ApplyForce(-Damping);
	}
}

void UThreeBodyProblemSubsystem::ApplyForces(float DeltaTime)
{
	int32 N = Bodies.Num();
	if (N < 2) return;

	for (int32 i = 0; i < N; ++i)
	{
		for (int32 j = i + 1; j < N; ++j)
		{
			CalculateGravitationalForces(Bodies[i], Bodies[j]);
		}
	}
}