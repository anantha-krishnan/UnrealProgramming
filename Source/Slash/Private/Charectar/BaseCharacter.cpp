// Fill out your copyright notice in the Description page of Project Settings.


#include "Charectar/BaseCharacter.h"
#include "item/Weapons/Weapon.h"
#include "SlashComponents/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	USkeletalMeshComponent* LocalMesh = GetMesh();
	LocalMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LocalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	LocalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	LocalMesh->SetGenerateOverlapEvents(true);
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
	if (Trace== ECollisionEnabled::NoCollision)
		_EquippedWeapon->clearHitList();
}

void ABaseCharacter::RKeyPressed()
{
}

void ABaseCharacter::PlayMontage()
{
}
void ABaseCharacter::PlayMontage(UAnimInstance* AnimBP, UAnimMontage* animMont, FName montSec)
{
	AnimBP->Montage_Play(animMont);
	AnimBP->Montage_JumpToSection(montSec, animMont);
}

void ABaseCharacter::PlayAttackMontage()
{
	UAnimInstance* _AnimBP = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (_AnimBP && _AttackMontage && _AttackMontageNames.Num()>0)
	{
		FName montSecName;		int32 montSec;
		GetRandMontSec(_AttackMontageNames, montSec, montSecName);
		
		PlayMontage(_AnimBP, _AttackMontage, montSecName);
	}
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEndNotify()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack end notify in base character"));

}

void ABaseCharacter::PlayDieMontage()
{
	UAnimInstance* _AnimBP = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (_AnimBP && _DeathMontage && _DeathMontageNames.Num() > 0)
	{
		FName montSecName;		int32 montSec;
		GetRandMontSec(_DeathMontageNames, montSec, montSecName);
		PlayMontage(_AnimBP, _DeathMontage, montSecName);
	}
}

void ABaseCharacter::GetHit_Implementation(const FVector& hitloc)
{
	DirectionalHit(hitloc);
}

void ABaseCharacter::DirectionalHit(const FVector& hitloc)
{
	const FVector enemyfwd = GetActorForwardVector().GetSafeNormal2D();
	const FVector enemyloc = GetActorLocation();
	const FVector hitvec = (hitloc - enemyloc).GetSafeNormal2D();
	FName montsec = "FromBack";

	double theta = FMath::Acos(FVector::DotProduct(enemyfwd, hitvec));
	theta = FMath::RadiansToDegrees(theta);

	double zaxis = FVector::CrossProduct(enemyfwd, hitvec).GetSignVector().Z;
	theta = theta * zaxis;
	if (theta > -45.f && theta < 45.f)
		montsec = "FromFront";
	else if (theta > -135.f && theta < -45.f)
		montsec = "FromRight";
	else if (theta > -45.f && theta < 135.f)
		montsec = "FromLeft";
	if (_Attributes)
	{
		if (_Attributes->IsAlive())
			PlayHitReactMontage(montsec);
		else
			PlayDieMontage();
	}
}

void ABaseCharacter::GetRandMontSec(TArray<FName>& montArrayNames, int32& montSec, FName& montSecName)
{
	montSec = FMath::RandRange(0, montArrayNames.Num() - 1);
	montSecName = montArrayNames[montSec];
}

void ABaseCharacter::PlayHitReactMontage(FName montSec)
{
	UAnimInstance* AnimBP = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimBP && _HitMontage)
		PlayMontage(AnimBP, _HitMontage, montSec);		
}

void ABaseCharacter::PlayHitSound(const FVector& hitloc)
{
	if (_HitSound)
		UGameplayStatics::PlaySoundAtLocation(this, _HitSound, hitloc);
}

void ABaseCharacter::SpawnHitParticles(const FVector& hitloc)
{
	if (_HitBlood)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _HitBlood, hitloc);

}

void ABaseCharacter::reduceHealth(float danage)
{
	if (_Attributes)
	{
		_Attributes->reduceCurrHealth(danage);
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

