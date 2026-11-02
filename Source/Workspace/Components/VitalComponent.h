#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VitalComponent.generated.h"


/** Called when MaxValue changes. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVitalMaxValueChanged, float, NewMaxValue);

/** Called when CurrentValue changes. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVitalValueChanged, float, NewValue, float, OldValue);

/** Called when CurrentValue drops to zero. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVitalDepleted);

/** Called when CurrentValue rise above zero. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVitalRestored);

/** Called when CurrentValue rises above 5% of MaxValue. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVitalRecovered);

/** Called when CurrentValue reaches MaxValue. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVitalFilled);

/** Generic component holding a single vital stat (e.g. Health, Stamina, Sanity). */
UCLASS(ClassGroup=(Vitals), meta=(BlueprintSpawnableComponent))
class WORKSPACE_API UVitalComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	/** Maximum value. Set per instance in Details. */
	UPROPERTY(EditAnywhere, Category = "Vitals", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MaxValue{100.0f};

	/** Current value (read-only preview). Change it via Add, Remove or SetCurrentValue. */
	UPROPERTY(VisibleAnywhere, Category = "Vitals")
	float CurrentValue{100.0f};

protected:
	virtual void BeginPlay() override;

public:
	UVitalComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Fired after MaxValue has changed. */
	UPROPERTY(BlueprintAssignable, Category = "Vitals")
	FOnVitalMaxValueChanged OnMaxValueChanged;

	/** Fired after CurrentValue has changed. */
	UPROPERTY(BlueprintAssignable, Category = "Vitals")
	FOnVitalValueChanged OnValueChanged;

	/** Fired once when CurrentValue falls to zero. */
	UPROPERTY(BlueprintAssignable, Category = "Vitals")
	FOnVitalDepleted OnDepleted;

	/** Fired once when CurrentValue rise above zero. */
	UPROPERTY(BlueprintAssignable, Category = "Vitals")
	FOnVitalRestored OnRestored;

	/** Called when CurrentValue rises above 5% of MaxValue. */
	UPROPERTY(BlueprintAssignable, Category = "Vitals")
	FOnVitalRecovered OnRecovered;

	/** Fired once when CurrentValue reaches MaxValue. */
	UPROPERTY(BlueprintAssignable, Category = "Vitals")
	FOnVitalFilled OnFilled;

	/** Sets MaxValue to a random number between RangeMin and RangeMax. */
	UFUNCTION(BlueprintCallable, Category = "Vitals")
	void RandomizeMaxValue(float RangeMin, float RangeMax, bool bRoundToInteger = true);

	/** Sets a new MaxValue. CurrentValue is clamped if it exceeds it. */
	UFUNCTION(BlueprintCallable, Category = "Vitals")
	void SetMaxValue(float NewMaxValue);

	/** Returns MaxValue. */
	UFUNCTION(BlueprintPure, Category = "Vitals")
	float GetMaxValue() const;

	/** Sets CurrentValue, clamped between 0 and MaxValue. */
	UFUNCTION(BlueprintCallable, Category = "Vitals")
	void SetCurrentValue(float NewCurrentValue);

	/** Returns CurrentValue. */
	UFUNCTION(BlueprintPure, Category = "Vitals")
	float GetCurrentValue() const;

	/** Adds Amount to CurrentValue (up to MaxValue). */
	UFUNCTION(BlueprintCallable, Category = "Vitals")
	void Add(float Amount = 1.0f);

	/** Removes Amount from CurrentValue (down to 0). */
	UFUNCTION(BlueprintCallable, Category = "Vitals")
	void Remove(float Amount = 1.0f);

	/** Modifies the current value by the specified amount (clamped between 0 and MaxValue). */
	UFUNCTION(BlueprintCallable, Category = "Vitals")
	void ModifyByAmount(float Amount);

	/** Returns CurrentValue / MaxValue (0 to 1). Returns 0 if MaxValue is 0. */
	UFUNCTION(BlueprintPure, Category = "Vitals")
	float GetNormalizedValue() const;
};