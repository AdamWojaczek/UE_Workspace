// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/PatrolMode.h"
#include "PatrolComponent.generated.h"


UCLASS(ClassGroup=(Patrol), meta=(BlueprintSpawnableComponent))
class WORKSPACE_API UPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	int32 CurrentIndex{-1};
	int32 Direction{1};

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Patrol")
	TArray<TObjectPtr<AActor>> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
	EPatrolMode PatrolMode{EPatrolMode::Loop};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
	bool bDescendingOrder{false};

protected:
	virtual void BeginPlay() override;

	int32 GetNextIndex();

public:	
	UPatrolComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Patrol")
	void ResetPatrol();

	UFUNCTION(BlueprintCallable, Category = "Patrol")
	bool GetNextLocation(FVector& OutLocation);
};
