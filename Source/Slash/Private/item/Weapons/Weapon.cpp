// Fill out your copyright notice in the Description page of Project Settings.

#include "item/Weapons/Weapon.h"
#include "Charectar/SlashCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
    
    _BoxForTraceHit = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordHitTrace"));
    _BoxForTraceHit->SetupAttachment(GetRootComponent());
    
    _BoxForTraceHit->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    _BoxForTraceHit->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    _BoxForTraceHit->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    
    _BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
    _BoxTraceStart->SetupAttachment(GetRootComponent());
    _BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
    _BoxTraceEnd->SetupAttachment(GetRootComponent());

    
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();
    _BoxForTraceHit->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}
void AWeapon::Equip(USceneComponent* InParent, FName SocketName)
{
    FAttachmentTransformRules attRules(EAttachmentRule::SnapToTarget, true);
    ItemMesh->AttachToComponent(InParent, attRules, SocketName);
    Super::Equip(InParent, SocketName);
    if (SphereComp)
        SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (_WeaponEquipSound)
        UGameplayStatics::PlaySoundAtLocation(this, _WeaponEquipSound, GetActorLocation());
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

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    
    const FVector traceBoxS = _BoxTraceStart->GetComponentLocation();
    const FVector traceBoxE = _BoxTraceEnd->GetComponentLocation();
    TArray<AActor*>IgnoreList;
    FHitResult hit;
    IgnoreList.Add(this);
    UKismetSystemLibrary::BoxTraceSingle(
        this, 
        traceBoxS, 
        traceBoxE, 
        FVector(5.f, 5.f, 5.f), 
        _BoxTraceStart->GetComponentRotation(), 
        ETraceTypeQuery::TraceTypeQuery1, 
        false, 
        IgnoreList, 
        EDrawDebugTrace::ForDuration, 
        hit, 
        true);
    
}
