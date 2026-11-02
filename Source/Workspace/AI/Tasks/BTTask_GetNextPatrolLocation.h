#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetNextPatrolLocation.generated.h"

/** Behavior Tree task that selects the next patrol location and stores its position in the Blackboard. */
UCLASS(Blueprintable, HideCategories = ("Blackboard"))
class WORKSPACE_API UBTTask_GetNextPatrolLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	/** Returns the task description displayed in the Behavior Tree node. */
	virtual FString GetStaticDescription() const override;

protected:
	/** Blackboard key that receives the selected patrol location.*/
	UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
	FBlackboardKeySelector PatrolLocationKey;

public:
	/** Selects the next patrol location and writes its position to the Blackboard. */
	UBTTask_GetNextPatrolLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

