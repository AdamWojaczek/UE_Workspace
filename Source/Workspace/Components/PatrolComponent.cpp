#include "Components/PatrolComponent.h"

UPatrolComponent::UPatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPatrolComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetPatrol();
}

void UPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPatrolComponent::ResetPatrol()
{
	Direction = bDescendingOrder ? -1 : 1;
	CurrentIndex = bDescendingOrder ? PatrolPoints.Num() : -1;
}

bool UPatrolComponent::GetNextLocation(FVector& OutLocation)
{
	bool Result = false;

	const int32 NextIndex = GetNextIndex();
	
	if (PatrolPoints.IsValidIndex(NextIndex))
	{
		CurrentIndex = NextIndex;
		if (IsValid(PatrolPoints[NextIndex]))
		{
			OutLocation = PatrolPoints[NextIndex]->GetActorLocation();
			Result = true;
		}
	}

	return Result;
}

int32 UPatrolComponent::GetNextIndex()
{
	int32 NextIndex = -1;
	const int32 NumPatrolPoints = PatrolPoints.Num();
	
	if (NumPatrolPoints == 0)
		NextIndex = -1;
	else if (NumPatrolPoints == 1)
		NextIndex = (CurrentIndex != 0) ? 0 : -1;
	else
	{
		const int32 MaxIndex = NumPatrolPoints - 1;

		switch (PatrolMode)
		{
			case EPatrolMode::Random:
			{
				do
				{
					NextIndex = FMath::RandRange(0, MaxIndex);
				} while (NextIndex == CurrentIndex);
			} break;

			case EPatrolMode::Loop:
			{
				NextIndex = CurrentIndex + Direction;
				if (NextIndex > MaxIndex)
					NextIndex = 0;
				else if (NextIndex < 0)
					NextIndex = MaxIndex;
			} break;

			case EPatrolMode::PingPong:
			{
				NextIndex = CurrentIndex + Direction;
				if ((NextIndex >= MaxIndex) || (NextIndex <= 0))
					Direction = -Direction;			
			} break;

			default:
				NextIndex = -1;
		}
	}

	return NextIndex;
}