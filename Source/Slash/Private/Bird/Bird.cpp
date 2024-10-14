// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_Capsule = CreateDefaultSubobject< UCapsuleComponent>(TEXT("Capsule"));
	_Capsule->SetCapsuleHalfHeight(20.f);
	_Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(_Capsule);

	_BirdMesh = CreateDefaultSubobject< USkeletalMeshComponent>(TEXT("BirdMesh"));
	_BirdMesh->SetupAttachment(_Capsule);

	_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_SpringArm->TargetArmLength = 200.f;
	_SpringArm->SetupAttachment(_Capsule);

	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(_SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABird::MoveForward(float value)
{
	//check if controller of this pawn is possessed or not. Dont move unpossessed pawns
	if (Controller && value !=0.f)
	{
		FVector fwd = GetActorForwardVector();
		AddMovementInput(fwd, value);
	}
}

void ABird::Turn(float value)
{
	if (Controller && value != 0.f)

	AddControllerYawInput(value);
}

void ABird::LookUp(float value)
{
	if (Controller && value != 0.f)
		AddControllerPitchInput(value);
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABird::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABird::LookUp);

}

