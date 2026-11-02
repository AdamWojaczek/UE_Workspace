#include "Components/VitalComponent.h"

UVitalComponent::UVitalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UVitalComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVitalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UVitalComponent::RandomizeMaxValue(float RangeMin, float RangeMax, bool bRoundToInteger)
{
	RangeMin = FMath::Max(0.0f, RangeMin);
	RangeMax = FMath::Max(RangeMin, RangeMax);

	float NewMax = FMath::FRandRange(RangeMin, RangeMax);
	if (bRoundToInteger)
		NewMax = FMath::RoundToFloat(NewMax);

	SetMaxValue(NewMax);
}

void UVitalComponent::SetMaxValue(float NewMaxValue)
{
	NewMaxValue = FMath::Max(NewMaxValue, 0.0f);
	if (NewMaxValue != MaxValue)
	{
		MaxValue = NewMaxValue;
		OnMaxValueChanged.Broadcast(MaxValue);

		if (CurrentValue > MaxValue)
			SetCurrentValue(MaxValue);
	}
}

float UVitalComponent::GetMaxValue() const
{
	return MaxValue;
}

void UVitalComponent::SetCurrentValue(float NewCurrentValue)
{
	NewCurrentValue = FMath::Clamp(NewCurrentValue, 0.0f, MaxValue);
	if (CurrentValue != NewCurrentValue)
	{
		const float OldValue = CurrentValue;
		CurrentValue = NewCurrentValue;

		OnValueChanged.Broadcast(CurrentValue, OldValue);

		if ((CurrentValue <= 0.0f) && (OldValue > 0.0f))
			OnDepleted.Broadcast();
		else if ((CurrentValue >= MaxValue) && (OldValue < MaxValue))
			OnFilled.Broadcast();
		else
		{
			if ((CurrentValue > 0.0f) && (OldValue == 0.0f))
				OnRestored.Broadcast();
			if ((CurrentValue > (MaxValue * 0.05f)) && (OldValue <= (MaxValue * 0.05f)))
				OnRecovered.Broadcast();
		}
	}
}

float UVitalComponent::GetCurrentValue() const
{
	return CurrentValue;
}

void UVitalComponent::Add(float Amount)
{
	if (Amount > 0.0f)
		SetCurrentValue(CurrentValue + Amount);
}

void UVitalComponent::Remove(float Amount)
{
	if (Amount > 0.0f)
		SetCurrentValue(CurrentValue - Amount);
}

void UVitalComponent::ModifyByAmount(float Amount)
{
	SetCurrentValue(CurrentValue + Amount);
}

float UVitalComponent::GetNormalizedValue() const
{
	float Result = 0.0f;

	if (MaxValue > 0.0f)
		Result = CurrentValue / MaxValue;

	return Result;
}