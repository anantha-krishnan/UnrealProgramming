// Fill out your copyright notice in the Description page of Project Settings.


#include "item/item.h"
#include "Slash/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Charectar/SlashCharacter.h"
#include "NiagaraComponent.h"

// Sets default values
Aitem::Aitem() : _totalTime(0.f),_amp(2.f),_period(5.f),_unitGlAxisVec(0,0,1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetupAttachment(GetRootComponent());

	_EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	_EmbersEffect->SetupAttachment(GetRootComponent());

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
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &Aitem::OnSphereOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &Aitem::onSphereEndOverlap);
}

float Aitem::TransformSin(float theta)
{
	if(_ItemState == EItemState::EIS_Hovering)
		return _amp * FMath::Sin(theta * _period)/** DeltaTime*/;
	return 0.f;
}

float Aitem::TransformCos(float theta)
{
	if (_ItemState == EItemState::EIS_Hovering)
		float offset = _amp * FMath::Cos(theta * _period)/** DeltaTime*/;
	return 0.f;
}

void Aitem::RotateItemActor(FVector unitGlAxisVec, double deg, float deltaTime)
{	

	//return 0.0f;
}

void Aitem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Green, OtherActor->GetName());
	*/
	ASlashCharacter* slashcha = Cast<ASlashCharacter>(OtherActor);
	if (slashcha)
	{
		slashcha->SetOverlappingItem(this);
	}
}

void Aitem::onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActor->GetName());
	*/
	ASlashCharacter* slashcha = Cast<ASlashCharacter>(OtherActor);
	if (slashcha)
	{
		slashcha->SetOverlappingItem(NULL);
	}
}

void Aitem::Equip(USceneComponent* InParent, FName SocketName, AActor* newowner, APawn* newInstigator)
{
	_ItemState = EItemState::EIS_Equipped;
}

// Called every frame
void Aitem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_totalTime += DeltaTime;
	//AddActorWorldRotation(FQuat(_unitGlAxisVec, _deg * _totalTime / 180.0 * 22.0 / 7.0));
	//UE_LOG(LogTemp, Warning, TEXT("DeltaTime : %f"), DeltaTime);
	
	//float offset = _amp * FMath::Sin(_totalTime * _period)/** DeltaTime*/;
	//FVector loc		= GetActorLocation();
	//FVector actFwd	= GetActorForwardVector();
	//FVector locE	= loc + actFwd * 100.0f;
	//bool persist	= false;
	double offset = TransformSin(_totalTime);
	AddActorWorldOffset(FVector(0, 0, offset));
	//DRAW_SPHERE(loc,persist);
	//DRAW_VECTOR(loc, locE, persist);
}

