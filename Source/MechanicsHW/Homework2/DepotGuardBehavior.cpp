#include "DepotGuardBehavior.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "DepotGuard.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MechanicsHWCharacter.h"
#include "NavigationSystem.h"

UBTService_DepotAwareness::UBTService_DepotAwareness()
{
	NodeName = TEXT("Sense player and remember sprint noise");
	Interval = 0.2f;
	RandomDeviation = 0.0f;
	bCreateNodeInstance = true;
}

void UBTService_DepotAwareness::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();
	ADepotGuard* Guard = Controller ? Cast<ADepotGuard>(Controller->GetPawn()) : nullptr;
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Guard || !Blackboard)
	{
		return;
	}

	auto* Player = Cast<AMechanicsHWCharacter>(UGameplayStatics::GetPlayerPawn(Guard, 0));
	const bool bWasChasing = Blackboard->GetValueAsObject(DepotGuardKeys::TargetActor) != nullptr;
	const double Distance = Player ? Guard->GetDistanceTo(Player) : TNumericLimits<double>::Max();
	const double SightRange = bWasChasing ? 1650.0 : 1200.0;
	if (Player && Distance <= SightRange && Controller->LineOfSightTo(Player))
	{
		Blackboard->ClearValue(DepotGuardKeys::NoiseLocation);
		Blackboard->SetValueAsObject(DepotGuardKeys::TargetActor, Player);
		Guard->SetBehaviorState(INVTEXT("CHASE"), FColor::Red, 560.0f, Player);
		return;
	}

	if (bWasChasing)
	{
		Blackboard->ClearValue(DepotGuardKeys::TargetActor);
	}

	// Sprint footfalls travel farther than sight and can be heard behind a prop.
	const double Now = Guard->GetWorld()->GetTimeSeconds();
	if (!bWasChasing && Player && Distance <= 2400.0 && Player->bIsSprinting
		&& Player->GetVelocity().SizeSquared2D() >= FMath::Square(520.0)
		&& Player->GetCharacterMovement()->IsMovingOnGround())
	{
		const UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Guard->GetWorld());
		FNavLocation Noise;
		if (Nav && Nav->ProjectPointToNavigation(Player->GetActorLocation(), Noise, FVector(170.0, 170.0, 250.0)))
		{
			LastNoiseTime = Now;
			if (!Blackboard->IsVectorValueSet(DepotGuardKeys::NoiseLocation)
				|| FVector::DistSquared2D(Blackboard->GetValueAsVector(DepotGuardKeys::NoiseLocation), Noise.Location)
					>= FMath::Square(150.0))
			{
				Blackboard->SetValueAsVector(DepotGuardKeys::NoiseLocation, Noise.Location);
			}
		}
	}

	if (!Player || Now - LastNoiseTime >= 6.0)
	{
		Blackboard->ClearValue(DepotGuardKeys::NoiseLocation);
	}

	if (Blackboard->IsVectorValueSet(DepotGuardKeys::NoiseLocation))
	{
		Guard->SetBehaviorState(INVTEXT("INVESTIGATE"), FColor(255, 175, 30), 380.0f, Player);
	}
	else
	{
		Guard->SetBehaviorState(INVTEXT("PATROL"), FColor::Cyan, 250.0f, Player);
	}
}

UBTTask_DepotPatrolPoint::UBTTask_DepotPatrolPoint()
{
	NodeName = TEXT("Pick reachable point near spawn");
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_DepotPatrolPoint, BlackboardKey));
}

EBTNodeResult::Type UBTTask_DepotPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto* Controller = Cast<ADepotGuardController>(OwnerComp.GetAIOwner());
	const auto* Guard = Controller ? Cast<ADepotGuard>(Controller->GetPawn()) : nullptr;
	const UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(OwnerComp.GetWorld());
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Guard || !Nav || !Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	const FVector& Origin = Controller->GetPatrolOrigin();
	FNavLocation Start;
	FNavLocation Destination;
	if (!Nav->ProjectPointToNavigation(Origin, Start, FVector(170.0, 170.0, 250.0))
		|| !Nav->GetRandomReachablePointInRadius(Start.Location, Guard->PatrolRadius, Destination))
	{
		return EBTNodeResult::Failed;
	}

	Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination.Location);
	return EBTNodeResult::Succeeded;
}
