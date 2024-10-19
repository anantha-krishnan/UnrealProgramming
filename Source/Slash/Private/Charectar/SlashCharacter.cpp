// Fill out your copyright notice in the Description page of Project Settings.


#include "Charectar/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "item/item.h"
#include "item/Weapons/Weapon.h"

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
	if (wp)
	{
		wp->Equip(this->GetMesh(), FName(TEXT("RightHandSocket")));
		_CharectarState = ECharectarState::ECS_EquippedOneHandedWeapon;
	}
}

void ASlashCharacter::RKeyPressed()
{
	AWeapon* wp = Cast<AWeapon>(_OverlappingItem);
	if (wp)
	{
		_CharectarState = ECharectarState::ECS_EquippedRwoHandedWeapon;
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
	//PlayerInputComponent->BindAction(FName("AttackSlide2HW"), EInputEvent::IE_Pressed, this, &ASlashCharacter::RKeyPressed);
}

