// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "item/item.h"
#include "Treasure.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class SLASH_API ATreasure : public Aitem
{
	GENERATED_BODY()
public:
	ATreasure();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, Category = "TreasureProp")
	USoundBase* _PickUpSound;

	UPROPERTY(EditAnywhere, Category = "TreasureProp")
	float _GoldValue;
};
