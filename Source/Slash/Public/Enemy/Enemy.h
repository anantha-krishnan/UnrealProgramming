// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;
class USoundBase;

UCLASS()
class SLASH_API AEnemy : public ACharacter,public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& hitloc) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void PlayMontage(FName montSec);

private:	
	UPROPERTY(EditDefaultsOnly, Category = "HitEffects")
	UAnimMontage* _EnemyHitMontage;

	UPROPERTY(EditAnywhere, Category = "HitEffects")
	USoundBase* _EnemyHitSound;

	UPROPERTY(EditAnywhere, Category = "HitEffects")
	UParticleSystem* _EnemyHitBlood;
};
