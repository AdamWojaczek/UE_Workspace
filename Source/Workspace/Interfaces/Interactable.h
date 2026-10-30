#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

// Add interface functions to this class. This is the class that will be inherited to implement this interface.
class WORKSPACE_API IInteractable
{
	GENERATED_BODY()

public:
	/** Performs the interaction (e.g. opens a door). */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* Interactor);

	/** Returns whether the interaction is currently possible. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	bool CanInteract(AActor* Interactor) const;

	/** Returns the prompt text shown in the UI, e.g. "Open Door". */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	FText GetInteractionText() const;
};
