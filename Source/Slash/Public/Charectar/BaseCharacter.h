// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interfaces/HitInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class USoundBase;
class UAnimMontage;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void WeaponTraceStEnd(ECollisionEnabled::Type Trace);

	AWeapon* _EquippedWeapon;
	virtual void RKeyPressed();
	virtual void PlayMontage();
	virtual void PlayMontage(UAnimInstance* anim, UAnimMontage* animMont, FName monSec);
	virtual void PlayAttackMontage();
	virtual void PlayDieMontage();
	void GetHit_Implementation(const FVector& hitloc);
	void DirectionalHit(const FVector& ImpactPoint);
	void GetRandMontSec(TArray<FName>& montArrayNames, int32& montSec, FName& montSecName);
	
	virtual bool CanAttack();
	UFUNCTION(BlueprintCallable)
	virtual void AttackEndNotify();


	void PlayHitReactMontage(FName montSec);

	UPROPERTY(VisibleAnywhere, Category = "HitEffects")
	UAttributeComponent* _Attributes;

	UPROPERTY(EditAnywhere, Category = "HitEffects")
	USoundBase* _HitSound;

	UPROPERTY(EditAnywhere, Category = "HitEffects")
	UParticleSystem* _HitBlood;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* _AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* _HitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* _DeathMontage;
	
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> _AttackMontageNames;
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<FName> _DeathMontageNames;

	virtual void PlayHitSound(const FVector& hitloc);
	virtual void SpawnHitParticles(const FVector& hitloc);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
