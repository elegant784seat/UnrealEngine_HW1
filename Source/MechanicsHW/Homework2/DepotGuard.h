#pragma once

#include "AIController.h"
#include "GameFramework/Character.h"
#include "DepotGuard.generated.h"

class UBehaviorTree;
class UTextRenderComponent;

UCLASS()
class ADepotGuard : public ACharacter
{
	GENERATED_BODY()

public:
	ADepotGuard();

	UPROPERTY(EditAnywhere, Category = "Guard")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Guard", meta = (ClampMin = "100.0"))
	float PatrolRadius = 1550.0f;

	void SetBehaviorState(const FText& Label, const FColor& Color, float Speed, const AActor* Viewer);

private:
	UPROPERTY(VisibleAnywhere, Category = "Guard")
	TObjectPtr<UTextRenderComponent> StateLabel;
};

UCLASS()
class ADepotGuardController : public AAIController
{
	GENERATED_BODY()

public:
	const FVector& GetPatrolOrigin() const { return PatrolOrigin; }

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	FVector PatrolOrigin = FVector::ZeroVector;
};
