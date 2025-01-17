// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Charectar/CharectarTypes.h"
#include "../Charectar/BaseCharacter.h"
#include "Enemy.generated.h"

class UHealthWidgetComponent;
class AAIController;
class UPawnSensingComponent;
class APawn;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckCombatTarget();

	void SetChasingTrue();

	void SetPatrolTrue();

	void EnemyPatrolMovement();


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& hitloc) override;
	void SetHealthWidgetVisibility(bool status);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PlayDieMontage() override;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose _Pose = EDeathPose::EDP_Alive;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> _WeaponClass;
	virtual void AttackEndNotify() override;
	virtual bool CanAttack() override;
	bool CanChase();
private:

	UPROPERTY(VisibleAnywhere, Category = "HitEffects")
	UHealthWidgetComponent* _HealthWidget;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation")
	AActor* _CombatTarget;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double _CombatRadius;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double _AttackRadius;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* _PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*>_PatrolTargetsList;

	UPROPERTY()
	AAIController* _EnemyController;

	
	int32 _CurrPatrolTargetIndex;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation")
	bool _insideTargetRad;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation")
	double _disToTarget;

	FTimerHandle _PatrolDelayTimerHandle;
	UPROPERTY(VisibleAnywhere, Category = "AI Navigation")
	UPawnSensingComponent* _PawnSensing;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation")
	EEnemyState _CurrentState;


	UFUNCTION()
	void _PatrolDelayTimerCallBack();
	UPROPERTY(VisibleAnywhere, Category = "AI Navigation")
	double _GroundVelocity;
	void UpdatePatrolTarget();
	void MoveToTarget(AActor* target);

	UFUNCTION()
	void PawnSeen(APawn* Target);

	void updateHealthWidget(float per);
	void reduceHealth(float per);
	bool InTargetRange(AActor* target, double targetr);


};
