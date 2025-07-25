// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Body.generated.h"

UCLASS()
class CLICKINGBUTTONGAME_API ABody : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABody();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	// material
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMaterialInstanceDynamic* BodyMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physic Properties")
	float Mass = 10000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physic Properties")
	float GravitationalConstant = 6.67430e-11f; // Gravitational constant in m^3 kg^-1 s^-2

	// Initial velocity of the body
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physic Properties")
	FVector InitialVelocity = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Physic Properties")
	float VelocityLimit = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physic Properties")
	FVector Acceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physic Properties")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physic Properties")
	float RotationSpeed = 10.0f; // degrees per second

	UFUNCTION()
	void ApplyForce(FVector Force);

	UFUNCTION()
	void UpdatePosition(float DeltaTime);

private:
	FVector CurrentForce;
	FVector NetForce; // Accumulated force for the current frame
};
