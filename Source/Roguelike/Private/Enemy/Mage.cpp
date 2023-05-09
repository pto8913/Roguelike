// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Enemy/Mage.h"
#include "Public/Enemy/EnemyAIController.h"

#include "Public/Player/RoguelikeCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine.h"
#include "Public/RoguelikeGameInstance.h"

// Sets default values
AMage::AMage()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 100;

	GetCharacterMovement()->MaxWalkSpeed = 400;

	CapsuleComp = GetCapsuleComponent();
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AMage::OnComponentOverlap);
	CapsuleComp->ComponentTags.Add("Enemy");
	CapsuleComp->ComponentTags.Add("Mage");

	MeshComp = GetMesh();

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	FullHealth = 0.f;
	Health = 0.f;
	HealthPercentage = 0.f;
	HealthType = EHealthType::Full;

	Tags.Add("Enemy");
	Tags.Add("Mage");
}

// Called when the game starts or when spawned
void AMage::BeginPlay()
{
	Super::BeginPlay();

	if (InTutorial == true) return;

	if (FullHealth == 0.f)
	{
		FullHealth = 1000.f;
	}
	if (Health == 0.f)
	{
		Health = FullHealth;
	}
	if (HealthPercentage == 0.f)
	{
		HealthPercentage = 1.f;
	}
	bIsCanSeeUI = false;
	AController* MyController = GetController();
	if (MyController != nullptr)
	{
		AIController = Cast<AEnemyAIController>(MyController);
	}
	PlayAnimation(SpawnMontage);

	OpenEnemyUI(HealthPercentage);

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandler, this, &AMage::mGetNewLocation, 1.f, false, 3.f
	);
}

void AMage::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		CloseEnemyUI();
	}
}

void AMage::ActorSaveDataLoaded_Implementation(const float InFullHealth, const float InHealth)
{
	FullHealth = InFullHealth;
	Health = InHealth;
	HealthPercentage = Health / FullHealth;
	UpdateHealth(0);
}

void AMage::ActorSaveDataSaved_Implementation(float& OutFullHealth, float& OutHealth)
{
	OutFullHealth = FullHealth;
	OutHealth = Health;
}

void AMage::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (InTutorial == true) return;

	CheckIsInNotify(this);

	UpdateEnemyUIPos(this);

	if (bIsCanSeeUI == true)
	{
		if (Player != nullptr)
		{
			DistanceToTarget = GetHorizontalDistanceTo(Player);
			if (DistanceToTarget > 150 && DistanceToTarget <= 500)
			{
				StopAnimation();
				MoveSafety();
			}
			SetActorRotation(FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()));
		}
		return;
	}

	if (AIController != nullptr)
	{
		MoveToLocation(NewLocation);
	}
}

void AMage::mGetNewLocation()
{
	NewLocation = GetNewLocation(GetActorLocation());
	GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandler, this, &AMage::mGetNewLocation, 1.f, false, 3.f
	);
}

void AMage::OnComponentOverlap(
	class UPrimitiveComponent* OverlappedComponent,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (HealthType == EHealthType::Zero) return;

	if (OverlappedComponent->ComponentHasTag("Mage") == true)
	{
		if (OtherComp->ComponentHasTag("PlayerSword") == true || OtherActor->ActorHasTag("PlayerAttack") == true)
		{
			Player = Cast<ARoguelikeCharacter>(OtherComp->GetOwner());
			if (Player != nullptr)
			{
				if (Player->GetPlayerState() != EPlayerState::Attack) return;
			}
			PlayAnimation(HitMontage);
			bIsCanSeeUI = true;
			ForceNotifyTarget(this);
		}
		if (OtherComp->ComponentHasTag("EnemyNotifyArea") == true)
		{
			bIsCanSeeUI = true;
			ForceNotifyTarget(this);
		}
	}
}

void AMage::Summon()
{
	PlayAnimation(SummonMontage);
}

void AMage::mSummon()
{
	if (GameInstance != nullptr) {
		GameInstance->SpawnEnemy(1, false, GetActorLocation() + FVector(200.f, 0.f, 0.f));
	}
}

////////////////////////////////////////////////////////
// Status

float AMage::TakeDamage(
	float Damage,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	class AActor* DamageCauser
)
{
	if (InTutorial == true) return 0.f;

	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage)
	{
		ActualDamage = FMath::Abs(ActualDamage);
		UpdateHealth(-ActualDamage);
		if (HealthType == EHealthType::Zero)
		{
			PlayAnimation(DeathMontage);
		}
	}
	return ActualDamage;
}
