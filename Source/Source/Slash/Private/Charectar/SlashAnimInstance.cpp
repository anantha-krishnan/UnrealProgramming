// Fill out your copyright notice in the Description page of Project Settings.


#include "Charectar/SlashAnimInstance.h"
#include "Charectar/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    // fellow who has chosen this anim blueprint. 
    _SlashCharecter = Cast<ASlashCharacter>(TryGetPawnOwner());
    if (_SlashCharecter)
        _SlashCharecterMovComp = _SlashCharecter->GetCharacterMovement();
    
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);
    if (_SlashCharecterMovComp)
    {
        _GroundSpeed = UKismetMathLibrary::VSizeXY(_SlashCharecterMovComp->Velocity);
        _IsFalling = _SlashCharecterMovComp->IsFalling();
        _CharectarState = _SlashCharecter->GetCharectarState();
    }
}
