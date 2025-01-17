// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& hitloc) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* _BreakableGeometry;

	UPROPERTY(EditAnywhere, Category = "BreakableProperties")
	TArray<TSubclassOf<class ATreasure>> _TreasureClass;

	bool _broke;
};
