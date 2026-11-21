#include "AI/Tasks/BTTask_GetNextPatrolLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/PatrolComponent.h"

UBTTask_GetNextPatrolLocation::UBTTask_GetNextPatrolLocation()
{
	NodeName = TEXT("Get Next Patrol Location");
	PatrolLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, PatrolLocationKey));	
}

FString UBTTask_GetNextPatrolLocation::GetStaticDescription() const
{
	return Super::GetStaticDescription() +
		   TEXT("\n\n") + GetNodeName() + TEXT(": ") +
		   PatrolLocationKey.SelectedKeyName.ToString();
}

EBTNodeResult::Type UBTTask_GetNextPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	
	const AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* Pawn = IsValid(AIController) ? AIController->GetPawn() : nullptr;

	if (IsValid(Pawn))
	{
		UPatrolComponent* PatrolComp = Pawn->FindComponentByClass<UPatrolComponent>();
		if (IsValid(PatrolComp))
		{
			FVector NextLocation;
			if (PatrolComp->GetNextLocation(NextLocation))
			{
				UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
				if (IsValid(BlackboardComp))
				{
					BlackboardComp->SetValueAsVector(PatrolLocationKey.SelectedKeyName, NextLocation);
					Result = EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return Result;
}