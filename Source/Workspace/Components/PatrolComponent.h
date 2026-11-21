#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/PatrolMode.h"
#include "PatrolComponent.generated.h"

/**
 * Holds a list of patrol points and returns the next patrol location according to the selected
 * patrol mode (Loop, PingPong, Random) and ascending/descendig order.
 */
UCLASS(ClassGroup=(Patrol), meta=(BlueprintSpawnableComponent))
class WORKSPACE_API UPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** Index of the most recently returned patrol point (-1 = none). */
	int32 CurrentIndex{-1};

	/** Current traversal direction: 1 = ascending indices, -1 = descending. */
	int32 Direction{1};

	/** Actors (e.g. Target Points) marking the patrol route. Set per instance in the level. */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Patrol")
	TArray<TObjectPtr<AActor>> PatrolPoints;

	/** How the next point is chosen: Loop, PingPong or Random (enum). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
	EPatrolMode PatrolMode{EPatrolMode::Loop};

	/** Start direction: false = from the first point (ascending), true = from the last point (descending). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
	bool bDescendingOrder{false};

protected:
	virtual void BeginPlay() override;

	/** Applies the start-order settings to the internal state (direction and index). */
	void ApplyStartOrder();

	/** Computes the next point index for the current mode, or -1 if there are no points, errors etc. */
	int32 GetNextIndex();

public:	
	UPatrolComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Restarts the patrol using the configured start-order settings. */
	UFUNCTION(BlueprintCallable, Category = "Patrol")
	void ResetPatrol();

	/** Outputs the world location of the next patrol point. Returns false if no valid point is available. */
	UFUNCTION(BlueprintCallable, Category = "Patrol")
	bool GetNextLocation(FVector& OutLocation);
};
