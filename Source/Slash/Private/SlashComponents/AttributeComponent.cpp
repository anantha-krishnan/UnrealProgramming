// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashComponents/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent():_CurrentHealth(100.0),_MaxHealth(100.0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributeComponent::reduceCurrHealth(float damage)
{
	_CurrentHealth = FMath::Clamp(_CurrentHealth - damage, 0, _MaxHealth);

}

