// Fill out your copyright notice in the Description page of Project Settings.

#include "Breakables/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "item/Treasure.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABreakableActor::ABreakableActor():_broke(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	_BreakableGeometry = CreateDefaultSubobject< UGeometryCollectionComponent>(TEXT("GeometryCollectionMine"));
	SetRootComponent(_BreakableGeometry);
	_BreakableGeometry->SetGenerateOverlapEvents(true);
	_BreakableGeometry->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	_BreakableGeometry->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& hitloc)
{
	if (_broke)
		return;
	_broke = true;
	UWorld* thisWorld = GetWorld();
	if (thisWorld && _TreasureClass.Num()>0)
	{
		FVector potloc = GetActorLocation();
		potloc.Z = potloc.Z + 100.f;
		thisWorld->SpawnActor<ATreasure>(_TreasureClass[FMath::RandRange(0, _TreasureClass.Num() - 1)], potloc, GetActorRotation());
	}

}

