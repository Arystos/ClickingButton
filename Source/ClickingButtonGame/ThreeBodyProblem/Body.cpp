// Fill out your copyright notice in the Description page of Project Settings.


#include "Body.h"

#include "ThreeBodyProblemGameMode.h"


// Sets default values
ABody::ABody()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	BodyMesh->SetEnableGravity(false);
	BodyMesh->SetSimulatePhysics(true);
	BodyMesh->SetLinearDamping(0.0f);
	BodyMesh->SetAngularDamping(0.0f);
	BodyMesh->SetNotifyRigidBodyCollision(true);
	BodyMesh->SetCollisionProfileName(TEXT("PhysicsActor"));

	/*
	// Create and setup the mesh component
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	// properly create a dynamic material
	if (auto BaseMat = BodyMesh->GetMaterial(0))
	{
		BodyMaterial = UMaterialInstanceDynamic::Create(BaseMat, this);
		BodyMesh->SetMaterial(0, BodyMaterial);
	}

	BodyMesh->SetEnableGravity(false);
	BodyMesh->SetSimulatePhysics(true);
	BodyMesh->SetLinearDamping(0.0f);
	BodyMesh->SetAngularDamping(0.0f);
	*/
	
}

void ABody::BeginPlay()
{
	Super::BeginPlay();
	Velocity = InitialVelocity;
	RotationSpeed = FMath::RandRange(MinimumRotationSpeed, MaximumRotationSpeed);
}

void ABody::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Call RecalculateBodies to update the Bodies array in the game mode
	if (AThreeBodyProblemGameMode* GameMode = Cast<AThreeBodyProblemGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->RecalculateBodies();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast to AThreeBodyProblemGameMode"));
	}
}

void ABody::ApplyForce(FVector Force)
{
	NetForce += Force;
}

void ABody::UpdatePosition(float DeltaTime)
{
	if (Mass > 0)
	{
		// a = F / m
		Acceleration = NetForce / Mass;
	}

	// v = u + at
	Velocity += Acceleration * DeltaTime;

	// Clamp velocity to avoid numerical instability
	Velocity = Velocity.GetClampedToMaxSize(VelocityLimit);

	// Euler integration
	SetActorLocation(GetActorLocation() + Velocity * DeltaTime);
	
	// Clear the net force after applying it
	NetForce = FVector::ZeroVector;

	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
}