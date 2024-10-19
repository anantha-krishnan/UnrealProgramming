// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharectarTypes.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class Aitem;


UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
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
	void RKeyPressed();

private:
	ECharectarState _CharectarState = ECharectarState::ECS_Unequipped;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* _SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* _Camera;

	UPROPERTY(VisibleInstanceOnly)
	Aitem* _OverlappingItem;

public:
	FORCEINLINE void SetOverlappingItem(Aitem* item) { _OverlappingItem = item; }
	FORCEINLINE ECharectarState GetCharectarState() const { return _CharectarState; }
};
