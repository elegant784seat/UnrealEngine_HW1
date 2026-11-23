#include "DepotGuard.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ADepotGuard::ADepotGuard()
{
	AIControllerClass = ADepotGuardController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bUseControllerRotationYaw = false;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0, 540.0, 0.0);
	Movement->MaxWalkSpeed = 250.0f;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -88.0), FRotator(0.0, -90.0, 0.0));

	StateLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("StateLabel"));
	StateLabel->SetupAttachment(GetRootComponent());
	StateLabel->SetAbsolute(false, true, false);
	StateLabel->SetRelativeLocation(FVector(0.0, 0.0, 128.0));
	StateLabel->SetHorizontalAlignment(EHTA_Center);
	StateLabel->SetWorldSize(28.0f);
	StateLabel->SetText(INVTEXT("PATROL"));
	StateLabel->SetTextRenderColor(FColor::Cyan);
}

void ADepotGuard::SetBehaviorState(const FText& Label, const FColor& Color, float Speed, const AActor* Viewer)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	if (Viewer)
	{
		const FVector ToViewer = Viewer->GetActorLocation() - GetActorLocation();
		StateLabel->SetWorldRotation(FRotator(0.0, ToViewer.Rotation().Yaw, 0.0));
	}
	if (!StateLabel->Text.EqualTo(Label))
	{
		StateLabel->SetText(Label);
		StateLabel->SetTextRenderColor(Color);
	}
}

void ADepotGuardController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const ADepotGuard* Guard = Cast<ADepotGuard>(InPawn);
	if (Guard && Guard->BehaviorTree)
	{
		PatrolOrigin = Guard->GetActorLocation();
		RunBehaviorTree(Guard->BehaviorTree);
	}
}
