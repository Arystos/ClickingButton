// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Body.h"
#include "GameFramework/GameModeBase.h"
#include "ThreeBodyProblemGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CLICKINGBUTTONGAME_API AThreeBodyProblemGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AThreeBodyProblemGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/// @brief Applies gravitational forces between bodies and updates their positions.
	/// @param DeltaTime The time elapsed since the last frame, used for updating positions.
	UFUNCTION(BlueprintCallable, Category = "Physics")
	void ApplyForces(float DeltaTime);

	/// @brief Fills the Bodies array with all Body actors present in the level.
	UFUNCTION(BlueprintCallable, Category = "Physics")
	TArray<ABody*> FillArrayOfBodies();

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void InitializeOrbit(ABody* BodyA, ABody* BodyB);

private:
	/// @brief Calculates the gravitational forces between two bodies and applies them.
	///	Is used in ApplyForces.
	void CalculateGravitationalForces(ABody* BodyA, ABody* BodyB);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta=(AllowPrivateAccess="true"))
	TArray<ABody*> Bodies; // Array to hold all bodies in the level

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta=(AllowPrivateAccess="true"))
	float MaxDistance = 1000.0f; // Maximum distance between bodies before applying damping forces

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta=(AllowPrivateAccess="true", ToolTip="Global gravitational constant"))
	float FriendshipConstant = 2.0f; // Control the strength of the damping effect

	// Softening factor to prevent singularities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta=(AllowPrivateAccess="true"))
	float Softening = 0.1f; // Softening factor to prevent singularities in gravitational calculations

};
