// Fill out your copyright notice in the Description page of Project Settings.


#include "item/Treasure.h"
#include "Charectar/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"

ATreasure::ATreasure():_GoldValue(5.f)
{

}

void ATreasure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//_totalTime += DeltaTime;
	//AddActorWorldRotation(FQuat(_unitGlAxisVec, _deg * _totalTime / 180.0 * 22.0 / 7.0));

}

void ATreasure::onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASlashCharacter* slashcha = Cast<ASlashCharacter>(OtherActor);
	if (slashcha)
	{
		if (_PickUpSound)
			UGameplayStatics::PlaySoundAtLocation(this, _PickUpSound, GetActorLocation());
		Destroy();
	}
}
