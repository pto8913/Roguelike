// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Public/Player/RoguelikeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/WorldSettings.h"

#include "Public/UMG/OptionWidget.h"
#include "Public/UMG/PlayerWidget.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "Engine.h"

#include "Public/RoguelikeGameInstance.h"
#include "Public/RoguelikeGameMode.h"
#include "Public/ScoreGameMode.h"
#include "Public/MazeGameMode.h"

//////////////////////////////////////////////////////////////////////////
// ARoguelikeCharacter

ARoguelikeCharacter::ARoguelikeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->ComponentTags.Add("Player");
	//GetCapsuleComponent()->SetCollisionProfileName("OverlapAllDynamic");

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARoguelikeCharacter::OnComponentOverlap);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	MeshComp = GetMesh();
	AnimInstance = MeshComp->GetAnimInstance();
	
	/* sword collision is in blueprint */

	Ability_E_Boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Ability_E Boom"));
	Ability_E_Boom->SetupAttachment(MeshComp);
	Ability_E_Boom->SetRelativeLocation(FVector(250.f, 0.f, 0.f));
	Ability_E_Boom->TargetArmLength = 450.0f;
	Ability_E_Boom->bUsePawnControlRotation = false;
	Ability_E_Boom->bInheritPitch = false;
	Ability_E_Boom->bInheritRoll = false;
	Ability_E_Boom->bDoCollisionTest = false;
	Ability_E_Boom->ComponentTags.Add("Ignore");

	Ability_E_TeleportTarget = CreateDefaultSubobject<USphereComponent>(TEXT("Ability_E_TeleportTarget Comp"));
	Ability_E_TeleportTarget->SetupAttachment(Ability_E_Boom);
	Ability_E_TeleportTarget->SetRelativeLocation(FVector(550.f, 0.f, 0.f));
	Ability_E_TeleportTarget->OnComponentBeginOverlap.AddDynamic(this, &ARoguelikeCharacter::OnComponentOverlap);
	Ability_E_TeleportTarget->ComponentTags.Add("Ability_E_TeleportTarget");
	Ability_E_TeleportTarget->ComponentTags.Add("Ignore");

	Ability_F_ExplosionTarget = CreateDefaultSubobject<USphereComponent>(TEXT("Ability_F_ExplosionTarget Comp"));
	Ability_F_ExplosionTarget->SetupAttachment(MeshComp);
	Ability_F_ExplosionTarget->SetRelativeLocation(FVector(0.f, -250.f, 100.f));

	Ability_R_Area = CreateDefaultSubobject<UBoxComponent>(TEXT("Ability_R_Area Comp"));
	Ability_R_Area->SetupAttachment(Ability_E_Boom);
	Ability_R_Area->SetCollisionProfileName("OverlapAllDynamic");
	Ability_R_Area->ComponentTags.Add("Ability_R");
	Ability_R_Area->ComponentTags.Add("Ignore");
	Ability_R_Deactivate();

	EnemyNotifyArea = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyNotifyArea Comp"));
	EnemyNotifyArea->SetupAttachment(Ability_E_Boom);
	EnemyNotifyArea->ComponentTags.Add("EnemyNotifyArea");
	EnemyNotifyArea->ComponentTags.Add("Ignore");
	EnemyNotifyArea_Deactive();

	HealthType = EHealthType::Full;

	Tags.Add("Player");
}

void ARoguelikeCharacter::BeginPlay()
{
	Super::BeginPlay();

	float Duration = PlayAnimation(SpawnMontage);

	if (AScoreGameMode::GetGameMode() != nullptr)
	{
		if (AnimInstance != nullptr)
		{
			FTimerHandle h;
			GetWorld()->GetTimerManager().SetTimer(h, this, &ARoguelikeCharacter::PlayIdle, 1.f, false, Duration);
		}
	}
	else
	{
		if (ARoguelikeGameMode::GetGameMode() == nullptr && AMazeGameMode::GetGameMode() == nullptr) return;

		if (GetGameInstance() != nullptr)
		{
			EnemyNotifyArea_Activate();
			GameInstance->SetPlayer(this);
			PlayerUI = GameInstance->GetPlayerUI();
			if (GameInstance->GetPlayerFullHealth() != 0.f)
			{
				FullHealth = GameInstance->GetPlayerFullHealth();
			}
			if (GameInstance->GetPlayerHealth() != 0.f)
			{
				Health = GameInstance->GetPlayerHealth();
				HealthPercentage = Health / FullHealth;
			}
			if (PlayerUI != nullptr)
			{
				PlayerUI->OpenPlayerUI(HealthPercentage);
			}
		}
	}
}

void ARoguelikeCharacter::OpenOption()
{
	if (GetGameInstance() != nullptr)
	{
		OptionUI = GameInstance->GetOptionUI();
		if (OptionUI != nullptr)
		{
			if (!OptionUI->IsInViewport())
			{
				OptionUI->OpenOption();
			}
			else
			{
				OptionUI->CloseOption();
			}
		}
	}
}

void ARoguelikeCharacter::OpenMap()
{
	if (GetGameInstance() != nullptr)
	{
		MapUI = GameInstance->GetMapUI();
		if (MapUI != nullptr)
		{
			if (!MapUI->IsInViewport())
			{
				MapUI->OpenMap();
			}
			else
			{
				MapUI->CloseMap();
			}
		}
	}
}

void ARoguelikeCharacter::PlayIdle()
{
	if (AnimInstance != nullptr)
	{
		AnimInstance->Montage_Play(Idle, 1.f, EMontagePlayReturnType::MontageLength, 0.f, false);
		AnimInstance->Montage_SetNextSection("Finish", "Default", Idle);
	}
}

void ARoguelikeCharacter::OnComponentOverlap(
	class UPrimitiveComponent* OverlappedComponent,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (HealthType == EHealthType::Zero) return;

	if (OverlappedComponent->ComponentHasTag("PlayerSword") == true)
	{
		if (OtherActor->ActorHasTag("Player") == true) return;
		if (OtherComp->ComponentHasTag("Player") == true) return;

		float Damage = 0.f;
		if (IsPlaying(PrimaryAMontage) == true || IsPlaying(PrimaryBMontage) == true || IsPlaying(PrimaryCMontage) == true)
		{
			Damage = NormalDamage;
		}
		else if (IsPlaying(Ability_EMontage) == true)
		{
			Damage = Ability_EDamage;
		}
		else if (IsPlaying(Ability_QMontage) == true)
		{
			Damage = Ability_QDamage;
		}
		else if (IsPlaying(Ability_RMontage) == true)
		{
			Damage = Ability_RDamage;
		}

		APlayerController* TController = Cast<APlayerController>(OtherActor);
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);

		OtherActor->TakeDamage(Damage, DamageEvent, TController, this);
	}
	if (OverlappedComponent->ComponentHasTag("Ability_R") == true)
	{
		if (OtherActor->ActorHasTag("Player") == true) return;
		if (OtherComp->ComponentHasTag("Player") == true) return;

		APlayerController* TController = Cast<APlayerController>(OtherActor);
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);

		OtherActor->TakeDamage(100.f, DamageEvent, TController, this);
	}
	if (OverlappedComponent->ComponentHasTag("Player") == true)
	{
		if (OtherComp->ComponentHasTag("EnemyAttack") == true || OtherActor->ActorHasTag("EnemyAttack") == true)
		{
			PlayAnimation(HitMontage);
		}
		if (OtherComp->ComponentHasTag("Portal") == true || OtherActor->ActorHasTag("Portal") == true)
		{
			if (GetGameInstance() != nullptr)
			{
				GameInstance->SetPlayerFullHealth(FullHealth);
				GameInstance->SetPlayerHealth(Health);
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// 

void ARoguelikeCharacter::EnemyNotifyArea_Deactive()
{
	EnemyNotifyArea->SetRelativeScale3D({ 1.f, 1.f, 0.1f });
}

void ARoguelikeCharacter::EnemyNotifyArea_Activate()
{
	EnemyNotifyArea->SetRelativeScale3D({ 50.f, 50.f, 0.1f });
}


//////////////////////////////////////////////////////////////////////////
// Attack

void ARoguelikeCharacter::Attack()
{
	CurrentState = EPlayerState::Attack;
	if (bComboAttackSave == true)
	{
		ComboAttack %= 3;
		if (ComboAttack == 0)
		{
			PlayAnimation(PrimaryAMontage);
		}
		else if (ComboAttack == 1)
		{
			PlayAnimation(PrimaryBMontage);
		}
		else if (ComboAttack == 2)
		{
			PlayAnimation(PrimaryCMontage);
		}
		++ComboAttack;
	}
	else if (bComboAttackSave == false)
	{
		ComboAttack = 0;
		bComboAttackSave = true;
	}
}

void ARoguelikeCharacter::Ability_E()
{
	PlayAnimation(Ability_EMontage);
}

void ARoguelikeCharacter::Ability_E_Implementation()
{
	CurrentState = EPlayerState::Attack;
	FVector TelepoLocation = Ability_E_TeleportTarget->GetComponentLocation();

	TeleportTo(
		TelepoLocation,
		GetActorRotation()
	);
}

void ARoguelikeCharacter::Ability_Q()
{
	CurrentState = EPlayerState::Attack;
	PlayAnimation(Ability_QMontage);
}

void ARoguelikeCharacter::Ability_R()
{
	CurrentState = EPlayerState::Attack;
	Ability_R_Activate();
	PlayAnimation(Ability_RMontage);
}

void ARoguelikeCharacter::Ability_R_Activate()
{
	Ability_R_Area->SetRelativeScale3D(FVector(5.f, 5.f, 0.1f));
}

void ARoguelikeCharacter::Ability_R_Deactivate()
{
	Ability_R_Area->SetRelativeScale3D(FVector(1.f, 1.f, 0.1f));
}

void ARoguelikeCharacter::Ability_F()
{
	CurrentState = EPlayerState::Attack;
	Ability_FDamage = 100.f;

	PlayAnimation(Ability_FMontage);
}

void ARoguelikeCharacter::Ability_F_Finish()
{
	AnimInstance->Montage_Stop(1.f, Ability_FMontage);

	PlayAnimation(Ability_F_FinishMontage);
}

void ARoguelikeCharacter::Ability_F_Charge(float In)
{
	if (In != 0.f)
	{
		if (!IsPlaying(Ability_FMontage))
		{
			Ability_F_Finish();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Status

float ARoguelikeCharacter::TakeDamage(
	float Damage,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	class AActor* DamageCauser
)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage)
	{
		ActualDamage = FMath::Abs(ActualDamage);
		UpdateHealth(-ActualDamage);
	}
	return ActualDamage;
}

void ARoguelikeCharacter::UpdateHealth(float ChangeHelath)
{
	Health += ChangeHelath;
	Health = FMath::Clamp(Health, 0.f, FullHealth);
	HealthPercentage = Health / FullHealth;
	HealthType = UpdateHealthType(HealthPercentage);
	if (GetGameInstance() != nullptr)
	{
		GameInstance->SetPlayerFullHealth(FullHealth);
		GameInstance->SetPlayerHealth(Health);
		if (PlayerUI != nullptr)
		{
			PlayerUI->UpdateHealth(HealthPercentage);
			if (HealthType == EHealthType::Zero)
			{
				PlayerUI->ClosePlayerUI();
				GameInstance->SetInputMode(FInputModeUIOnly());

				GetCapsuleComponent()->ComponentTags.Empty();
				Tags.Empty();

				GetWorld()->GetWorldSettings()->SetTimeDilation(0.2f);
				PlayAnimation(DeathMontage);
			}
		}
	}
}

EHealthType ARoguelikeCharacter::UpdateHealthType(const float InHealthPercentage)
{
	if (InHealthPercentage > 0.5f)
	{
		return EHealthType::Full;
	}
	if (InHealthPercentage > 0.f && InHealthPercentage <= 0.5f)
	{
		return EHealthType::Half;
	}
	if (InHealthPercentage == 0.f)
	{
		return EHealthType::Zero;
	}

	return EHealthType::None;
}

//////////////////////////////////////////////////////////////////////////
// Animation

float ARoguelikeCharacter::PlayAnimation(UAnimMontage* PlayMontage)
{
	MeshComp = GetMesh();
	if (MeshComp != nullptr)
	{
		AnimInstance = MeshComp->GetAnimInstance();
	}
	if (AnimInstance != nullptr)
	{
		if (PlayMontage != nullptr)
		{
			if (AnimInstance->IsAnyMontagePlaying() == true)
			{
				UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
				if (CurrentMontage != nullptr)
				{
					FString AnimName = CurrentMontage->GetName();
					if (AnimName == "Aurora_Ability_F_Finish" || AnimName == "Aurora_Ability_F")
					{
						// Force play
						if (PlayMontage->GetName() != "Aurora_Ability_F_Finish")
						{
							AnimInstance->Montage_Stop(1.f, PlayMontage);
						}
					}
					else if (AnimName == "Aurora_Ability_R")
					{
						Ability_R_Deactivate();
						AnimInstance->Montage_Stop(1.f, CurrentMontage);
					}
					else if (AnimName == "Aurora_DeathMontage")
					{
						return 0.f;
					}
					else
					{
						AnimInstance->Montage_Stop(1.f, CurrentMontage);
					}
				}
			}
			return AnimInstance->Montage_Play(PlayMontage);
		}
	}
	return 0.f;
}

bool ARoguelikeCharacter::IsPlaying(UAnimMontage* PlayMontage)
{
	if (PlayMontage == nullptr || AnimInstance == nullptr) return false;
	return AnimInstance->Montage_IsPlaying(PlayMontage);
}

//////////////////////////////////////////////////////////////////////////
// Templates

URoguelikeGameInstance* ARoguelikeCharacter::GetGameInstance()
{
	return GameInstance = URoguelikeGameInstance::GetGameInstance();
}


//////////////////////////////////////////////////////////////////////////
// Input

void ARoguelikeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("OpenOption", IE_Pressed, this, &ARoguelikeCharacter::OpenOption);
	PlayerInputComponent->BindAction("OpenMap", IE_Pressed, this, &ARoguelikeCharacter::OpenMap);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ARoguelikeCharacter::Attack);

	PlayerInputComponent->BindAction("Ability_Q", IE_Pressed, this, &ARoguelikeCharacter::Ability_Q);
	PlayerInputComponent->BindAction("Ability_E", IE_Pressed, this, &ARoguelikeCharacter::Ability_E);
	PlayerInputComponent->BindAction("Ability_R", IE_Pressed, this, &ARoguelikeCharacter::Ability_R);

	PlayerInputComponent->BindAction("Ability_F", IE_Pressed, this, &ARoguelikeCharacter::Ability_F);
	PlayerInputComponent->BindAction("Ability_F", IE_Released, this, &ARoguelikeCharacter::Ability_F_Finish);
	PlayerInputComponent->BindAxis("Ability_F_Charge", this, &ARoguelikeCharacter::Ability_F_Charge);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARoguelikeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARoguelikeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARoguelikeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARoguelikeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARoguelikeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARoguelikeCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARoguelikeCharacter::OnResetVR);
}

void ARoguelikeCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARoguelikeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ARoguelikeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ARoguelikeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARoguelikeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARoguelikeCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARoguelikeCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
