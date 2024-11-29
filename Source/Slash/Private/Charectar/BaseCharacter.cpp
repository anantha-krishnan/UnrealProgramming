// Fill out your copyright notice in the Description page of Project Settings.


#include "Charectar/BaseCharacter.h"
#include "item/Weapons/Weapon.h"
#include "SlashComponents/AttributeComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::WeaponTraceStEnd(ECollisionEnabled::Type Trace)
{
	if (_EquippedWeapon)
		_EquippedWeapon->SetWeaponCollision(Trace);
	_EquippedWeapon->clearHitList();
}

void ABaseCharacter::RKeyPressed()
{
}

void ABaseCharacter::PlayMontage()
{
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEndNotify()
{
}

void ABaseCharacter::PlayDieMontage()
{
}

void ABaseCharacter::PlayHitReactMontage(FName montSec)
{
	UAnimInstance* _EnemyAnimBP = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (_EnemyAnimBP && _HitMontage)
	{
		_EnemyAnimBP->Montage_Play(_HitMontage);
		_EnemyAnimBP->Montage_JumpToSection(montSec, _HitMontage);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

