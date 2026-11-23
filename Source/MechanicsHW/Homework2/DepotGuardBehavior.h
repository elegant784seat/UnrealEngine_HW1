#pragma once

#include "BehaviorTree/BTService.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "DepotGuardBehavior.generated.h"

namespace DepotGuardKeys
{
	inline const FName TargetActor(TEXT("TargetActor"));
	inline const FName NoiseLocation(TEXT("NoiseLocation"));
	inline const FName PatrolLocation(TEXT("PatrolLocation"));
}

UCLASS()
class UBTService_DepotAwareness : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DepotAwareness();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	double LastNoiseTime = 0.0;
};

UCLASS()
class UBTTask_DepotPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_DepotPatrolPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
