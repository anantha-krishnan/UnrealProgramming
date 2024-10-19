// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "item.generated.h"

class USphereComponent;

UCLASS()
class SLASH_API Aitem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aitem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UFUNCTION(BlueprintCallable)
	UFUNCTION(BlueprintPure)
	float TransformSin(float theta);

	UFUNCTION(BlueprintPure)
	float TransformCos(float theta);

	UFUNCTION(BlueprintCallable)
	void RotateItemActor(FVector unitGlAxisVec, double deg, float deltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SineParam")
	float _period;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SineParam")
	float _amp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotationParams")
	FVector _unitGlAxisVec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotationParams")
	double _deg;

	UFUNCTION(BlueprintCallable)
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="true"))
	float _totalTime;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;
};
