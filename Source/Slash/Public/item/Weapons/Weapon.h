// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "item/item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;
class USceneComponent;


/**
 * 
 */
UCLASS()
class SLASH_API AWeapon : public Aitem
{
	GENERATED_BODY()
	
public:
	AWeapon();
	void Equip(USceneComponent* InParent, FName SocketName) override;
	void BeginPlay() override;
	void SetWeaponCollision(ECollisionEnabled::Type CollisionType);

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	USoundBase* _WeaponEquipSound;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	UBoxComponent* _BoxForTraceHit;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	USceneComponent* _BoxTraceStart;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	USceneComponent* _BoxTraceEnd;

	UFUNCTION(BlueprintCallable)
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	


};
