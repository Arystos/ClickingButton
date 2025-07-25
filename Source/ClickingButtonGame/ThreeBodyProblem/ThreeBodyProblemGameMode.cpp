// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreeBodyProblemGameMode.h"
#include "GameFramework/Actor.h" 
#include "EngineUtils.h"

AThreeBodyProblemGameMode::AThreeBodyProblemGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AThreeBodyProblemGameMode::BeginPlay()
{
	Super::BeginPlay();
	Bodies.Empty();			// Clear the array of bodies before filling it
	FillArrayOfBodies();	// Find all bodies in the level
}

void AThreeBodyProblemGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ApplyForces(DeltaTime);
	
	// Update the position of each body based on the calculated forces
	for (ABody* Body : Bodies)
	{
		if (Body)
			Body->UpdatePosition(DeltaTime);
	}
}

TArray<ABody*> AThreeBodyProblemGameMode::FillArrayOfBodies()
{
	// Find all the body actors in the level and add them to the Bodies array
	Bodies.Empty();
	for (TActorIterator<ABody> It(GetWorld()); It; ++It)
	{
		Bodies.Add(*It);
	}
	UE_LOG(LogTemp, Warning, TEXT("Initialized %d bodies"), Bodies.Num());

	// return the array of bodies
	return Bodies;
}

void AThreeBodyProblemGameMode::InitializeOrbit(ABody* BodyA, ABody* BodyB)
{
	if (BodyA && BodyB)
	{
		FVector Distance = BodyB->GetActorLocation() - BodyA->GetActorLocation();
		float DistanceMagnitude = Distance.Size();
		DistanceMagnitude = FMath::Max(DistanceMagnitude, 1.0f); // Avoid division by zero

		// Calculate the tangential velocity
		float VelocityMagnitude = FMath::Sqrt((BodyA->GravitationalConstant * BodyB->Mass) / DistanceMagnitude);

		// Find a perpendicular direction for the velocity
		FVector TangentialDirection = FVector::CrossProduct(Distance, FVector::UpVector).GetSafeNormal();

		// Set initial velocities perpendicular to the distance between the bodies
		BodyA->Velocity = TangentialDirection * VelocityMagnitude;
		BodyB->Velocity = -TangentialDirection * VelocityMagnitude; // Opposite direction for orbit

		// Optionally, you can add a bit of randomness to the velocity to avoid perfect circular orbits
		BodyA->Velocity += FVector(FMath::RandRange(-25.0f, 25.0f), FMath::RandRange(-25.0f, 25.0f), 0.0f);
		BodyB->Velocity += FVector(FMath::RandRange(-25.0f, 25.0f), FMath::RandRange(-25.0f, 25.0f), 0.0f);
	}
}

void AThreeBodyProblemGameMode::ApplyForces(float DeltaTime)
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

void AThreeBodyProblemGameMode::CalculateGravitationalForces(ABody* A, ABody* B)
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
