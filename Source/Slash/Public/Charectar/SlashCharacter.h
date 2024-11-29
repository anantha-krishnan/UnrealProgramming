// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharectarTypes.h"
#include "../Charectar/BaseCharacter.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class Aitem;
class UAnimInstance;
class UAnimMontage;
class AWeapon;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	void Turn(float value);

	void LookUp(float value);
	void EKeyPressed();
	virtual void RKeyPressed() override;
	virtual void PlayMontage() override;

	virtual bool CanAttack() override;

	bool CanDisarm();

	bool CanArm();

	virtual void AttackEndNotify() override;

	UFUNCTION(BlueprintCallable)
	void WeaponDisarmSocketNotify();

	UFUNCTION(BlueprintCallable)
	void WeaponArmSocketNotify();

	UFUNCTION(BlueprintCallable)
	void WeaponArmDisarmNotify();

private:
	ECharectarState _CharectarState = ECharectarState::ECS_Unequipped;
	EActionState _ActionState = EActionState::EAS_UnOccupied;	

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* _SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* _Camera;

	UPROPERTY(VisibleInstanceOnly, Category = "Weapon")
	Aitem* _OverlappingItem;
	


	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* _SlashDisarmMontage;


public:
	FORCEINLINE void SetOverlappingItem(Aitem* item) { _OverlappingItem = item; }
	FORCEINLINE ECharectarState GetCharectarState() const { return _CharectarState; }
};
