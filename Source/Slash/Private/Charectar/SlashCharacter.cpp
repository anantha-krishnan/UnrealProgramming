// Fill out your copyright notice in the Description page of Project Settings.


#include "Charectar/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "item/item.h"
#include "item/Weapons/Weapon.h"
#include "Animation/AnimInstance.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "SlashComponents/AttributeComponent.h"

// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_SpringArm->TargetArmLength = 200.f;
	_SpringArm->SetupAttachment(GetRootComponent());

	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(_SpringArm);



}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName(UEnum::GetDisplayValueAsText(ECharectars::EC_SlashCharectar).ToString()));

	InitialiseSlashOverlay();

}

void ASlashCharacter::InitialiseSlashOverlay()
{
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		ASlashHUD* sh = Cast<ASlashHUD>(pc->GetHUD());
		if (sh)
		{
			shov = sh->GetSlashOverlay();
			if (shov && _Attributes)
			{
				shov->SetHealthBarPercent(_Attributes->getHealthPercent());
				shov->SetStaminaBarPercent(1.f);
				shov->SetGold(0);
				shov->SetSouls(0);
			}
		}
	}
}

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::MoveForward(float value)
{
	if (_ActionState== EActionState::EAS_UnOccupied)
	//check if controller of this pawn is possessed or not. Dont move unpossessed pawns
	if (Controller && value != 0.f)
	{
		FRotator t =  GetControlRotation();
		FVector fwd = FRotationMatrix(FRotator(0.f, t.Yaw, 0.f)).GetUnitAxis(EAxis::X);
		AddMovementInput(fwd, value);
	}
}

void ASlashCharacter::MoveRight(float value)
{
	if (_ActionState == EActionState::EAS_UnOccupied)
		//check if controller of this pawn is possessed or not. Dont move unpossessed pawns
	if (Controller && value != 0.f)
	{
		FRotator t = GetControlRotation();
		FVector rt = FRotationMatrix(FRotator(0.f, t.Yaw, 0.f)).GetUnitAxis(EAxis::Y);
		AddMovementInput(rt, value);
	}
}

void ASlashCharacter::Turn(float value)
{
	if (Controller && value != 0.f)
		AddControllerYawInput(value);
}

void ASlashCharacter::LookUp(float value)
{
	if (Controller && value != 0.f)
		AddControllerPitchInput(value);
}

void ASlashCharacter::EKeyPressed()
{
	AWeapon* wp = Cast<AWeapon>(_OverlappingItem);
	if (wp && !_EquippedWeapon)
	{
		wp->Equip(this->GetMesh(), FName(TEXT("RightHandSocket")),this,this);
		_CharectarState = ECharectarState::ECS_EquippedOneHandedWeapon;
		_OverlappingItem = nullptr;
		_EquippedWeapon = wp;
	}
	else
	{
		UAnimInstance* _SlashAnimBP = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
		if (CanDisarm())
		{
			if (_SlashAnimBP && _SlashDisarmMontage)
			{
				FName montSec = TEXT("At_Disarm");
				_SlashAnimBP->Montage_Play(_SlashDisarmMontage);
				_SlashAnimBP->Montage_JumpToSection(montSec, _SlashDisarmMontage);
				_ActionState = EActionState::EAS_ArmDisarm;
			}
			_CharectarState = ECharectarState::ECS_Unequipped;
		}
		else if(CanArm())
		{
			if (_SlashAnimBP && _SlashDisarmMontage)
			{
				FName montSec = TEXT("At_Arm");
				_SlashAnimBP->Montage_Play(_SlashDisarmMontage);
				_SlashAnimBP->Montage_JumpToSection(montSec, _SlashDisarmMontage);
				_ActionState = EActionState::EAS_ArmDisarm;
			}
			_CharectarState = ECharectarState::ECS_EquippedOneHandedWeapon;
		}
	}

}

void ASlashCharacter::RKeyPressed()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		_ActionState = EActionState::EAS_Attacking;
	}
}

bool ASlashCharacter::CanAttack()
{
	return _ActionState == EActionState::EAS_UnOccupied &&
		_CharectarState != ECharectarState::ECS_Unequipped &&
		_EquippedWeapon;
}

bool ASlashCharacter::CanDisarm()
{
	return CanAttack();
	return (_EquippedWeapon && _ActionState == EActionState::EAS_UnOccupied && _CharectarState != ECharectarState::ECS_Unequipped);
}

bool ASlashCharacter::CanArm()
{
	return (_EquippedWeapon && _ActionState == EActionState::EAS_UnOccupied && _CharectarState == ECharectarState::ECS_Unequipped);
}

void ASlashCharacter::AttackEndNotify()
{
	_ActionState = EActionState::EAS_UnOccupied;
}

void ASlashCharacter::GetHit_Implementation(const FVector& hitloc)
{
	Super::GetHit_Implementation(hitloc);
	PlayHitSound(hitloc);
	SpawnHitParticles(hitloc);
}

void ASlashCharacter::WeaponDisarmSocketNotify()
{
	_EquippedWeapon->Equip(this->GetMesh(), FName(TEXT("spine_05Socket")),this,this);
}

void ASlashCharacter::WeaponArmSocketNotify()
{
	_EquippedWeapon->Equip(this->GetMesh(), FName(TEXT("RightHandSocket")),this,this);
}

void ASlashCharacter::WeaponArmDisarmNotify()
{
	_ActionState = EActionState::EAS_UnOccupied;
}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASlashCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);
	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), EInputEvent::IE_Pressed, this, &ASlashCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), EInputEvent::IE_Pressed, this, &ASlashCharacter::RKeyPressed);
}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::reduceHealth(DamageAmount);
	if (shov && _Attributes)
		shov->SetHealthBarPercent(_Attributes->getHealthPercent());
	return DamageAmount;
}
