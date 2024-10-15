// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SlashAnimInstance.generated.h"

class ASlashCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	ASlashCharacter* _SlashCharecter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	UCharacterMovementComponent* _SlashCharecterMovComp;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float _GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool _IsFalling;
};
