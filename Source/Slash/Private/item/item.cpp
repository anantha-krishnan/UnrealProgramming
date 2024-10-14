// Fill out your copyright notice in the Description page of Project Settings.


#include "item/item.h"
#include "Slash/DebugMacros.h"

// Sets default values
Aitem::Aitem() : _totalTime(0.f),_amp(1.f),_period(1.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMeshComponent");
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void Aitem::BeginPlay()
{
	Super::BeginPlay();
	UWorld* W = GetWorld();
	//_amp = 10.f;
	//_period = 10.f;
	//if (W)
	{
		//if (GEngine)
		{
			FVector loc		= GetActorLocation();
			FVector actFwd	= GetActorForwardVector();
			FVector locEnd = loc + actFwd * 100.0;
			FQuat actQat = GetActorQuat();
			//FString actorName = GetName();
			//DRAW_SPHERE(loc);
			/*DRAW_LINE(loc, locEnd);
			DRAW_POINT(loc);*/
			//DRAW_VECTOR(loc, locEnd);
			//FString time = FString::Printf(TEXT("delta time %f"), DeltaTime);
			//GEngine->AddOnScreenDebugMessage(1, 10.0, FColor::Magenta, time);
			//UE_LOG(LogTemp, Warning, TEXT("Item name : %s"), *actorName);
		}
	}
}

float Aitem::TransformSin(float theta)
{
	float offset = _amp * FMath::Sin(theta * _period)/** DeltaTime*/;
	return offset;
}

float Aitem::TransformCos(float theta)
{
	float offset = _amp * FMath::Cos(theta * _period)/** DeltaTime*/;
	return offset;
}

void Aitem::RotateItemActor(FVector unitGlAxisVec, double deg, float deltaTime)
{	

	AddActorWorldRotation(FQuat(unitGlAxisVec, deg* deltaTime / 180.0 * 22.0 / 7.0));
	//return 0.0f;
}

// Called every frame
void Aitem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_totalTime += DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("DeltaTime : %f"), DeltaTime);
	
	//float offset = _amp * FMath::Sin(_totalTime * _period)/** DeltaTime*/;
	FVector loc		= GetActorLocation();
	FVector actFwd	= GetActorForwardVector();
	FVector locE	= loc + actFwd * 100.0f;
	bool persist	= false;
	//AddActorWorldOffset(FVector(0, 0, offset));
	DRAW_SPHERE(loc,persist);
	DRAW_VECTOR(loc, locE, persist);
}

