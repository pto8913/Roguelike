// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Enemy/Swordman.h"
#include "Public/Enemy/EnemyAIController.h"

#include "Public/Player/RoguelikeCharacter.h"

#include "Perception/PawnSensingComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"

#include "TimerManager.h"
#include "Engine.h"

// Sets default values
ASwordman::ASwordman()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 100;

	GetCharacterMovement()->MaxWalkSpeed = 500;

	CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCapsuleHalfHeight(122.f);
	CapsuleComp->SetCapsuleRadius(62.f);
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ASwordman::OnComponentOverlap);
	CapsuleComp->ComponentTags.Add("Enemy");
	CapsuleComp->ComponentTags.Add("Swordman");

	/* sword / shield collsion component is in bp. because can't attach socket. */

	MeshComp = GetMesh();

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	FullHealth = 0.f;
	Health = 0.f;
	HealthPercentage = 0.f;
	HealthType = EHealthType::Full;

	Tags.Add("Enemy");
	Tags.Add("Swordman");
}

// Called when the game starts or when spawned
void ASwordman::BeginPlay()
{
	Super::BeginPlay();

	if (InTutorial == true) return;

	if (FullHealth == 0.f)
	{
		FullHealth = 2000.f;
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
		TimerHandler, this, &ASwordman::mGetNewLocation, 1.f, false, 3.f
	);
}

void ASwordman::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		CloseEnemyUI();
	}
}

void ASwordman::ActorSaveDataLoaded_Implementation(const float InFullHealth, const float InHealth)
{
	FullHealth = InFullHealth;
	Health = InHealth;
	HealthPercentage = Health / FullHealth;
	UpdateHealth(0);
}

void ASwordman::ActorSaveDataSaved_Implementation(float& OutFullHealth, float& OutHealth)
{
	OutFullHealth = FullHealth;
	OutHealth = Health;
}

void ASwordman::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (InTutorial == true) return;

	CheckIsInNotify(this);

	UpdateEnemyUIPos(this);

	if (bIsCanSeeUI == true)
	{
		AIController->bIsFindPlayer = true;
		if (Player != nullptr)
		{
			DistanceToTarget = GetHorizontalDistanceTo(Player);
			if (DistanceToTarget > 350)
			{
				MoveToTarget();
				SetActorRotation(FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()));
			}
		}
		return;
	}

	if (AIController != nullptr)
	{
		MoveToLocation(NewLocation);
	}
}

void ASwordman::mGetNewLocation()
{
	NewLocation = GetNewLocation(GetActorLocation());
	GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandler, this, &ASwordman::mGetNewLocation, 1.f, false, 3.f
	);
}

void ASwordman::OnComponentOverlap(
	class UPrimitiveComponent* OverlappedComponent,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (HealthType == EHealthType::Zero) return;

	if (OverlappedComponent->ComponentHasTag("Shield") == true)
	{
		if (OtherActor->ActorHasTag("PlayerAttack") == true || OtherComp->ComponentHasTag("PlayerAttack"))
		{
			if (BlockCount < 2)
			{
				++BlockCount;
				float tmp = FMath::RandRange(0, 2);
				int RndAttack = ((int)tmp + (int)(tmp * 10) % 10) % 2;
				switch (RndAttack)
				{
				case 0:
					PlayAnimation(ShieldBlockMontage);
					break;
				default:
					PlayAnimation(ShieldBashMontage);
					break;
				}
			}
			else
			{
				// hit
				PlayAnimation(HitMontage);
			}
		}
	}
	if (OverlappedComponent->ComponentHasTag("Sword") == true)
	{
		if (OtherComp->ComponentHasTag("Ignore") == true) return;

		if (OtherActor->ActorHasTag("Player") == true || OtherComp->ComponentHasTag("Player") == true)
		{
			float Damage = 0.f;
			if (IsPlayingAnim(SmallSwingAttackMontage) == true)
			{
				Damage = SmallSwingDamage;
			}
			else if (IsPlayingAnim(BigSwingAttackMontage) == true)
			{
				Damage = BigSwingDamage;
			}
			else if (IsPlayingAnim(FrenzieAttackMontage) == true)
			{
				Damage = FrenzieDamage;
			}

			APlayerController* TController = Cast<APlayerController>(OtherActor);
			TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
			FDamageEvent DamageEvent(ValidDamageTypeClass);

			OtherActor->TakeDamage(Damage, DamageEvent, TController, this);
		}
	}
	if (OverlappedComponent->ComponentHasTag("Swordman") == true)
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


////////////////////////////////////////////////////////
// Status

float ASwordman::TakeDamage(
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
