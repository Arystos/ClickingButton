// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ThreeBodyProblemSubsystem.generated.h"

class ABody;
/**
 * 
 */
UCLASS()
class CLICKINGBUTTONGAME_API UThreeBodyProblemSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
public:
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsAllowedToTick() const override;
	virtual bool IsTickable() const override;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;

	/// @brief Applies gravitational forces between bodies and updates their positions.
	/// @param DeltaTime The time elapsed since the last frame, used for updating positions.
	UFUNCTION(BlueprintCallable, Category = "Physics")
	void ApplyForces(float DeltaTime);

	/// @brief Fills the Bodies array with all Body actors present in the level.
	UFUNCTION(BlueprintCallable, Category = "Physics")
	TArray<ABody*> FillArrayOfBodies();
	
	/// @brief Recalculate the bodies in the level and update the Bodies array.
	UFUNCTION(BlueprintCallable, Category= "Physics")
	TArray<ABody*> RecalculateBodies();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta=(AllowPrivateAccess="true"))
	TArray<ABody*> Bodies; // Array to hold all bodies in the level

private:
	GENERATED_BODY()

	/// @brief Calculates the gravitational forces between two bodies and applies them.
	///	Is used in ApplyForces.
	void CalculateGravitationalForces(ABody* BodyA, ABody* BodyB);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta=(AllowPrivateAccess="true"))
	float MaxDistance = 100.0f; // Maximum distance between bodies before applying damping forces

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta=(AllowPrivateAccess="true", ToolTip="Global gravitational constant"))
	float FriendshipConstant = 1.0f; // Control the strength of the damping effect

	// Softening factor to prevent singularities
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta=(AllowPrivateAccess="true"))
	float Softening = 0.0f; // Softening factor to prevent singularities in gravitational calculations
};
