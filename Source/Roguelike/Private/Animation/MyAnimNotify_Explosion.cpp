// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Animation/MyAnimNotify_Explosion.h"
#include "Components/SphereComponent.h"
#include "Engine.h"

void UMyAnimNotify_Explosion::Notify(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation)
{
	MeshComp = InMeshComp;
	AnimName = Animation->GetName();

	Mage = Cast<AMage>(InMeshComp->GetOwner());
	if (Mage != nullptr)
	{
		Explosion(Mage);
		return;
	}

	Player = Cast<ARoguelikeCharacter>(InMeshComp->GetOwner());
	if (Player != nullptr)
	{
		Explosion(Player);
	}
}

void UMyAnimNotify_Explosion::SpawnExplosion()
{
	if (MeshComp != nullptr)
	{
		world = MeshComp->GetWorld();
		if (world != nullptr)
		{
			Explode = world->SpawnActorDeferred<AExplosion>(AExplosion::StaticClass(), Trans);
		}
	}
}

void UMyAnimNotify_Explosion::Explosion(AMage* InOwner)
{
	ExplosionSpawnLocation = InOwner->GetActorLocation();
	ExplosionSpawnRotation = InOwner->GetActorRotation();

	SpawnExplosion();

	if (Explode != nullptr)
	{
		Explode->AttackAbleActorTag = "Player";
		Explode->Tags.Add("EnemyAtack");
		Explode->SetOwner(InOwner);

		if (AnimName == "Mage_BlastMontage")
		{
			Explode->ExplosionDamage = InOwner->BlastDamage;
			Explode->ParticleSysComponent->SetTemplate(InOwner->BlastParticle);
		}

		Trans = {
			ExplosionSpawnRotation,
			ExplosionSpawnLocation,
			ExplosionScale
		};

		Explode->FinishSpawning(Trans);
	}
}

void UMyAnimNotify_Explosion::Explosion(ARoguelikeCharacter* InOwner)
{
	SpawnExplosion();

	if (Explode != nullptr)
	{
		Explode->AttackAbleActorTag = "Enemy";
		Explode->Tags.Add("PlayerAttack");
		Explode->SetOwner(InOwner);

		if (AnimName == "Aurora_Ability_R")
		{
			Explode->ExplosionDamage = InOwner->ExplosionDamage;
			Player->GetCharacterMovement()->JumpZVelocity = 400.f;
			Player->Ability_R_Deactivate();
			Trans.SetLocation(InOwner->GetActorLocation());
			Trans.SetRotation(FQuat(InOwner->GetActorRotation()));
			Trans.SetScale3D(FVector(0.5f, 0.5f, 0.5f));
		}
		if (AnimName == "Aurora_Ability_F_Finish")
		{
			Explode->ExplosionDamage = InOwner->GetAbility_FDamage();
			Trans.SetLocation(InOwner->Ability_F_ExplosionTarget->GetComponentLocation());
			Trans.SetScale3D(FVector(0.5f, 0.5f, 0.5f));
			Explode->ParticleSysComponent->SetTemplate(InOwner->Ability_F_Explosion);
		}

		Explode->FinishSpawning(Trans);
	}
}