// Fill out your copyright notice in the Description page of Project Settings.

#include "item/Weapons/Weapon.h"
#include "Charectar/SlashCharacter.h"

void AWeapon::Equip(USceneComponent* InParent, FName SocketName)
{
    FAttachmentTransformRules attRules(EAttachmentRule::SnapToTarget, true);
    ItemMesh->AttachToComponent(InParent, attRules, SocketName);

}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    
    /*
    ASlashCharacter* SlashCharectar = Cast<ASlashCharacter>(OtherActor);
    if (SlashCharectar)
    {
        FAttachmentTransformRules attRules (EAttachmentRule::SnapToTarget, true);
        ItemMesh->AttachToComponent(SlashCharectar->GetMesh(), attRules,FName("RightHandSocket"));
    }
    GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Black, this->GetName()+FString(TEXT(" overlapping with "))+ OtherActor->GetName());
    */
}

void AWeapon::onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::onSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
    //GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Black, this->GetName() + FString(TEXT("overlapping end with")) + OtherActor->GetName());

}
