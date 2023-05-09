// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Enemy/Crossbowman.h"
#include "Public/Enemy/EnemyAIController.h"

#include "Public/Player/RoguelikeCharacter.h"

#include "Perception/PawnSensingComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"

#include "TimerManager.h"
#include "Engine.h"

// Sets default values
ACrossbowman::ACrossbowman()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 100;

	GetCharacterMovement()->MaxWalkSpeed = 500;

	CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCapsuleHalfHeight(140.f);
	CapsuleComp->SetCapsuleRadius(98.f);
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ACrossbowman::OnComponentOverlap);
	CapsuleComp->ComponentTags.Add("Enemy");
	CapsuleComp->ComponentTags.Add("Crossbowman");
	/* crossbow / crossbow collsion component is in bp. because can't attach socket. */
	MeshComp = GetMesh();

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	FullHealth = 0.f;
	Health = 0.f;
	HealthPercentage = 0.f;
	HealthType = EHealthType::Full;

	Tags.Add("Enemy");
	Tags.Add("Crossbowman");
}

// Called when the game starts or when spawned
void ACrossbowman::BeginPlay()
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
		TimerHandler, this, &ACrossbowman::mGetNewLocation, 1.f, false, 3.f
	);
}

void ACrossbowman::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		CloseEnemyUI();
	}
}

void ACrossbowman::ActorSaveDataLoaded_Implementation(const float InFullHealth, const float InHealth)
{
	FullHealth = InFullHealth;
	Health = InHealth;
	HealthPercentage = Health / FullHealth;
	UpdateHealth(0);
}

void ACrossbowman::ActorSaveDataSaved_Implementation(float& OutFullHealth, float& OutHealth)
{
	OutFullHealth = FullHealth;
	OutHealth = Health;
}

void ACrossbowman::Tick(float DeltaSeconds)
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

void ACrossbowman::OnComponentOverlap(
	class UPrimitiveComponent* OverlappedComponent,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (HealthType == EHealthType::Zero) return;

	if (OverlappedComponent->ComponentHasTag("CrossbowAttack") == true)
	{
		if (OtherActor->ActorHasTag("Player") == true || OtherComp->ComponentHasTag("Player") == true)
		{
			float Damage = 0.f;
			if (IsPlayingAnim(WhackAttackMontage) == true)
			{
				Damage = WhackDamage;
			}

			APlayerController* TController = Cast<APlayerController>(OtherActor);
			TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
			FDamageEvent DamageEvent(ValidDamageTypeClass);

			OtherActor->TakeDamage(Damage, DamageEvent, TController, this);
		}
	}
	if (OverlappedComponent->ComponentHasTag("Crossbowman") == true)
	{
		if (OtherComp->ComponentHasTag("PlayerSword") == true || OtherActor->ActorHasTag("PlayerAttack"))
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


////////////////////////////////////////////////////////
// Move

void ACrossbowman::mGetNewLocation()
{
	NewLocation = GetNewLocation(GetActorLocation());
	GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandler, this, &ACrossbowman::mGetNewLocation, 1.f, false, 3.f
	);
}

////////////////////////////////////////////////////////
// Status

float ACrossbowman::TakeDamage(
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
