// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Animation/MyAnimNotify_Bullet.h"

#include "Public/Enemy/Crossbowman.h"
#include "Public/Enemy/Mage.h"

#include "Components/SkeletalMeshComponent.h"

#include "Engine.h"

void UMyAnimNotify_Bullet::Notify(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation)
{
	AnimName = Animation->GetName();
	MeshComp = InMeshComp;

	Mage = Cast<AMage>(MeshComp->GetOwner());
	if (Mage != nullptr)
	{
		Fire(Mage);
		return;
	}

	Crossbowman = Cast<ACrossbowman>(MeshComp->GetOwner());
	if (Crossbowman != nullptr)
	{
		Fire(Crossbowman);
	}
}

/////////////////////////////////////////////////////////////
// Bullet

void UMyAnimNotify_Bullet::SpawnBullet()
{
	if (MeshComp != nullptr)
	{
		world = MeshComp->GetWorld();
	}
	if (world != nullptr)
	{
		Bullet = world->SpawnActorDeferred<ABullet>(
			ABullet::StaticClass(),
			Trans
			);
	}

	if (Bullet != nullptr)
	{
		Bullet->AttackableActor = "Player";
		Bullet->Tags.Add("EnemyAttack");
	}
}

void UMyAnimNotify_Bullet::Fire(AMage* InOwner)
{
	if (InOwner == nullptr) return;

	if (MeshComp != nullptr)
	{
		FireSpawnLocation = MeshComp->GetBoneLocation("hand_l");
	}

	FireSpawnRotation = InOwner->GetActorRotation();
	FireScale = { 0.7f, 0.7f, 0.7f };
	SpawnBullet();

	if (Bullet != nullptr)
	{
		if (AnimName == "Mage_ChargeMontage")
		{
			Bullet->BulletDamage = InOwner->ChargeDamage;
			FireScale = FVector(1.5f, 1.5f, 1.5f);
		}
		else if (AnimName == "Mage_FireMontage")
		{
			Bullet->BulletDamage = InOwner->FireDamage;
		}
		else if (AnimName == "Mage_RapidFireMontage")
		{
			Bullet->BulletDamage = InOwner->RapidDamage;
			Bullet->ProjectileMovement->InitialSpeed = 1500.f;
			Bullet->ProjectileMovement->MaxSpeed = 1500.f;

			FireSpawnRotation += FRotator(0.f, -90.f, 0.f);
			FireSpawnRotation += FRotator(0.f, 90.f, 0.f);
		}

		int RndPart = FMath::RandRange(0, 2);
		if (RndPart == 0)
		{
			Bullet->ParticleSysComponent->SetTemplate(InOwner->FireParticle);
		}
		else if (RndPart == 1)
		{
			Bullet->ParticleSysComponent->SetTemplate(InOwner->WaterParticle);
		}
		else
		{
			Bullet->ParticleSysComponent->SetTemplate(InOwner->ThunderParticle);
		}

		Trans = {
			FireSpawnRotation,
			FireSpawnLocation,
			FireScale
		};
		Bullet->FinishSpawning(Trans);
	}
}


void UMyAnimNotify_Bullet::Fire(ACrossbowman* InOwner)
{
	if (InOwner == nullptr) return;

	if (MeshComp != nullptr)
	{
		FireSpawnLocation = MeshComp->GetSocketLocation("hand_rSocket");
	}

	FireSpawnRotation = InOwner->GetActorRotation();
	FireScale = { 0.7f, 0.7f, 0.7f };
	SpawnBullet();

	if (Bullet != nullptr)
	{
		if (AnimName == "Crossbowman_ShootMontage")
		{
			Bullet->BulletDamage = InOwner->NormalDamage;
			FireScale = { 1.f, 1.f, 1.f };
		}
		else if (AnimName == "Crossbowman_RapidShootMontage")
		{
			Bullet->BulletDamage = InOwner->RapidDamage;
			Bullet->ProjectileMovement->InitialSpeed = 1500.f;
			Bullet->ProjectileMovement->MaxSpeed = 1500.f;

			FireSpawnRotation += FRotator(0.f, -60.f, 0.f);
			FireSpawnRotation += FRotator(0.f, 60.f, 0.f);
		}

		int RndPart = FMath::RandRange(0, 2);
		if (RndPart == 0)
		{
			Bullet->ParticleSysComponent->SetTemplate(InOwner->FireParticle);
		}
		else if (RndPart == 1)
		{
			Bullet->ParticleSysComponent->SetTemplate(InOwner->WaterParticle);
		}
		else
		{
			Bullet->ParticleSysComponent->SetTemplate(InOwner->ThunderParticle);
		}

		Trans = {
			FireSpawnRotation,
			FireSpawnLocation,
			FireScale
		};
		Bullet->FinishSpawning(Trans);
	}
}

