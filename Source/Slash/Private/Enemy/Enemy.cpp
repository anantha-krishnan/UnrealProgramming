// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SlashComponents/AttributeComponent.h"
#include "HUD/HealthWidgetComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USkeletalMeshComponent* enemyMesh = GetMesh();
	enemyMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	enemyMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	enemyMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	enemyMesh->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	_Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	_HealthWidget = CreateDefaultSubobject<UHealthWidgetComponent>(TEXT("HealthBar"));
	_HealthWidget->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	updateHealthWidget(1.f);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& hitloc)
{
	const FVector enemyfwd =  GetActorForwardVector().GetSafeNormal2D();
	const FVector enemyloc = GetActorLocation();
	const FVector hitvec = (hitloc - enemyloc).GetSafeNormal2D();
	FName montsec = "FromBack";

	double theta = FMath::Acos(FVector::DotProduct(enemyfwd,hitvec));
	theta = FMath::RadiansToDegrees(theta);

	double zaxis = FVector::CrossProduct(enemyfwd, hitvec).GetSignVector().Z;
	theta = theta * zaxis;
	if (theta > -45.f && theta < 45.f)
		montsec = "FromFront";
	else if (theta > -135.f && theta < -45.f)
		montsec = "FromRight";
	else if (theta > -45.f && theta < 135.f)
		montsec = "FromLeft";

	UKismetSystemLibrary::DrawDebugArrow(this, enemyloc, enemyloc + enemyfwd * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, enemyloc, enemyloc + hitvec*60.f, 5.f, FColor::Green, 5.f);
	if (_Attributes)
	{
		if (_Attributes->IsAlive())
			PlayMontage(montsec);
		else
			PlayDieMontage();
	}
	if (_EnemyHitSound)
		UGameplayStatics::PlaySoundAtLocation(this, _EnemyHitSound, hitloc);
	if (_EnemyHitBlood)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _EnemyHitBlood, hitloc);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	reduceHealth(DamageAmount);
	
	return 0.0f;
}

void AEnemy::PlayMontage(FName montSec)
{
	UAnimInstance* _EnemyAnimBP = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (_EnemyAnimBP && _EnemyHitMontage)
	{
		_EnemyAnimBP->Montage_Play(_EnemyHitMontage);
		_EnemyAnimBP->Montage_JumpToSection(montSec, _EnemyHitMontage);
	}
}

void AEnemy::PlayDieMontage()
{
	UAnimInstance* _EnemyAnimBP = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (_EnemyAnimBP && _EnemyDeathMontage)
	{
		const int32 randSec = FMath::RandRange(0, 5);
		FName montSec;
		switch (randSec)
		{
		case 0:
			montSec = TEXT("Death1");
			_Pose = EDeathPose::EDP_Dead1;
			break;
		case 1:
			montSec = TEXT("Death2");
			_Pose = EDeathPose::EDP_Dead2;
			break;
		case 2:
			montSec = TEXT("Death3");
			_Pose = EDeathPose::EDP_Dead3;
			break;
		case 3:
			montSec = TEXT("Death4");
			_Pose = EDeathPose::EDP_Dead4;
			break;
		case 4:
			montSec = TEXT("Death5");
			_Pose = EDeathPose::EDP_Dead5;
			break;
		case 5:
			montSec = TEXT("Death6");
			_Pose = EDeathPose::EDP_Dead6;
			break;
		default:
			montSec = TEXT("Death1");
			_Pose = EDeathPose::EDP_Dead1;
			break;
		}
		_EnemyAnimBP->Montage_Play(_EnemyDeathMontage);
		_EnemyAnimBP->Montage_JumpToSection(montSec, _EnemyDeathMontage);
	}
}

void AEnemy::reduceHealth(float danage)
{
	if (_Attributes)
	{
		_Attributes->reduceCurrHealth(danage);
		updateHealthWidget(_Attributes->getHealthPercent());
	}
	
}
void AEnemy::updateHealthWidget(float per)
{
	if (_HealthWidget)
		_HealthWidget->SetHealthPercent(per);
}

