// Fill out your copyright notice in the Description page of Project Settings.


#include "Charectar/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "item/item.h"
#include "item/Weapons/Weapon.h"
#include "Animation/AnimInstance.h"

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
		wp->Equip(this->GetMesh(), FName(TEXT("RightHandSocket")));
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
		PlayMontage();
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

void ASlashCharacter::WeaponDisarmSocketNotify()
{
	_EquippedWeapon->Equip(this->GetMesh(), FName(TEXT("spine_05Socket")));
}

void ASlashCharacter::WeaponArmSocketNotify()
{
	_EquippedWeapon->Equip(this->GetMesh(), FName(TEXT("RightHandSocket")));
}

void ASlashCharacter::WeaponArmDisarmNotify()
{
	_ActionState = EActionState::EAS_UnOccupied;
}

void ASlashCharacter::PlayMontage()
{
	UAnimInstance* _SlashAnimBP = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (_SlashAnimBP && _SlashAttackMontage)
	{
		const int32 randSec = FMath::RandRange(0, 1);
		FName montSec;
		switch (randSec)
		{
		case 0:
			montSec = TEXT("At_Horizontal");
			break;
		case 1:
			montSec = TEXT("At_360d");
			break;
		case 2:
			montSec = TEXT("At_Slide360d");
			break;
		default:
			montSec = TEXT("At_Horizontal");
			break;
		}
		_SlashAnimBP->Montage_Play(_SlashAttackMontage);
		_SlashAnimBP->Montage_JumpToSection(montSec, _SlashAttackMontage);
	}
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

