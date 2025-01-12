// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SlashComponents/AttributeComponent.h"
#include "HUD/HealthWidgetComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "item/Weapons/Weapon.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*USkeletalMeshComponent* enemyMesh = GetMesh();
	enemyMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	enemyMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	enemyMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	enemyMesh->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	*/
	_HealthWidget = CreateDefaultSubobject<UHealthWidgetComponent>(TEXT("HealthBar"));
	_HealthWidget->SetupAttachment(GetRootComponent());

	_CurrPatrolTargetIndex = 0;
	_CombatRadius = 600.f;
	_AttackRadius = 100.f;

	_PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensedPawn"));
	_PawnSensing->SightRadius = 4000.f;
	_PawnSensing->SetPeripheralVisionAngle(45.f);
	
	Tags.Add(FName(UEnum::GetDisplayValueAsText(ECharectars::EC_Enemy).ToString()));


}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	_CurrentState = EEnemyState::EES_Patrolling;

	SetHealthWidgetVisibility(false);
	updateHealthWidget(1.f);
	_EnemyController = Cast<AAIController>(GetController());
	/*
	for(auto p:_PatrolTargetsList)
		DrawDebugSphere(GetWorld(), p->GetActorLocation(), 150.f, 10, FColor::Magenta, true);
		*/
	if (_PatrolTargetsList.Num() > 0)
	{
		_PatrolTarget = _PatrolTargetsList[_CurrPatrolTargetIndex];
		
		EnemyPatrolMovement();
	}
	if (_PawnSensing)
		_PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	if (_WeaponClass)
	{
		AWeapon* wp = GetWorld()->SpawnActor<AWeapon>(_WeaponClass);
		if (wp)
		{
			wp->Equip(this->GetMesh(), FName(TEXT("RightHandSocket")), this, this);
			_EquippedWeapon = wp;
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_GroundVelocity = GetMovementComponent()->Velocity.Size2D();

	if (_CombatTarget)
		CheckCombatTarget();
	else
		UpdatePatrolTarget();

	//EnemyPatrolMovement();

}

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(_CombatTarget, _CombatRadius))
	{
		_CombatTarget = nullptr;
		SetHealthWidgetVisibility(false);
		SetPatrolTrue();
		UE_LOG(LogTemp, Warning, TEXT("Lost interest"));

	}
	else if (CanChase())
	{
		UE_LOG(LogTemp, Warning, TEXT("Chasing player"));
		SetChasingTrue();
	}
	else if(CanAttack())
	{		
		_CurrentState = EEnemyState::EES_Attacking;
		UE_LOG(LogTemp, Warning, TEXT("Attacking player"));
		PlayAttackMontage();
	}
	//_HealthWidget->SetVisibility(true);
}

void AEnemy::SetChasingTrue()
{
	GetWorldTimerManager().ClearTimer(_PatrolDelayTimerHandle);
	_CurrentState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	MoveToTarget(_CombatTarget);
}

void AEnemy::SetPatrolTrue()
{
	_CurrentState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = 125.f;
	MoveToTarget(_PatrolTarget);
}

void AEnemy::EnemyPatrolMovement()
{
	if (!_PatrolTarget || !_EnemyController)
		return;
	if (_GroundVelocity > 0.f)
		return;
	MoveToTarget(_PatrolTarget);
}


bool AEnemy::InTargetRange(AActor*target, double targetr)
{
	if (!target)
		return false;
	_disToTarget = (target->GetActorLocation() - GetActorLocation()).Size();
	if (_disToTarget <= targetr)
		return true;
	else
		return false;
}

void AEnemy::_PatrolDelayTimerCallBack()
{
	EnemyPatrolMovement();
}

void AEnemy::UpdatePatrolTarget()
{
	if (_CurrentState != EEnemyState::EES_Patrolling)
		return;
	_insideTargetRad = InTargetRange(_PatrolTarget, 150.f);
	if (_insideTargetRad)
	{
		if (_PatrolTargetsList.Num() > 0)
		{
			_CurrPatrolTargetIndex = (_CurrPatrolTargetIndex + 1) % _PatrolTargetsList.Num();
			_PatrolTarget = _PatrolTargetsList[_CurrPatrolTargetIndex];
		}
		GetWorldTimerManager().SetTimer(_PatrolDelayTimerHandle, this, &AEnemy::_PatrolDelayTimerCallBack, 5.f);
	}

	/*TArray<FNavPathPoint>pathpts = nvp->GetPathPoints();
	for (auto p : pathpts)
	{
		DrawDebugSphere(GetWorld(), p.Location, 15.f, 10, FColor::Magenta, true);
	}*/

}

void AEnemy::MoveToTarget(AActor* target)
{
	FAIMoveRequest movreq;
	movreq.SetGoalActor(target);
	movreq.SetAcceptanceRadius(30.f);
	FNavPathSharedPtr nvp;
	_EnemyController->MoveTo(movreq, &nvp);
}

void AEnemy::PawnSeen(APawn* Target)
{

	if (Target->ActorHasTag(FName(UEnum::GetDisplayValueAsText(ECharectars::EC_SlashCharectar).ToString())) 
		&& _CurrentState == EEnemyState::EES_Patrolling)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn seen, Chasing"));
		_CombatTarget = Target;
		SetChasingTrue();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& hitloc)
{
	Super::GetHit_Implementation(hitloc);
	SetHealthWidgetVisibility(true);
	

	//UKismetSystemLibrary::DrawDebugArrow(this, enemyloc, enemyloc + enemyfwd * 60.f, 5.f, FColor::Red, 5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, enemyloc, enemyloc + hitvec*60.f, 5.f, FColor::Green, 5.f);
	
	PlayHitSound(hitloc);
	SpawnHitParticles(hitloc);
}

void AEnemy::SetHealthWidgetVisibility(bool status)
{
	if (_HealthWidget)
		_HealthWidget->SetVisibility(status);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	reduceHealth(DamageAmount);
	_CombatTarget = EventInstigator->GetPawn();
	
	SetChasingTrue();
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (_EquippedWeapon)
		_EquippedWeapon->Destroy();
}


void AEnemy::PlayDieMontage()
{
	UAnimInstance* _AnimBP = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (_AnimBP && _DeathMontage && _DeathMontageNames.Num() > 0)
	{
		FName montSecName;		int32 montSec;
		GetRandMontSec(_DeathMontageNames, montSec, montSecName);
		_Pose = EDeathPose(montSec + 1);
		PlayMontage(_AnimBP, _DeathMontage, montSecName);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

void AEnemy::AttackEndNotify()
{

	UE_LOG(LogTemp, Warning, TEXT("End Attacking Slash"));
	_CurrentState = EEnemyState::EES_Patrolling;
}

bool AEnemy::CanAttack()
{
	if (InTargetRange(_CombatTarget, _AttackRadius) && _CurrentState != EEnemyState::EES_Attacking)
		return true;
	else
		return false;
}

bool AEnemy::CanChase()
{
	if (!InTargetRange(_CombatTarget, _AttackRadius) 
		&& _CurrentState < EEnemyState::EES_Chasing
		)
		return true;
	else
		return false;
}

void AEnemy::reduceHealth(float danage)
{
	Super::reduceHealth(danage);
	updateHealthWidget(_Attributes->getHealthPercent());

}
void AEnemy::updateHealthWidget(float per)
{
	if (_HealthWidget)
		_HealthWidget->SetHealthPercent(per);
}

