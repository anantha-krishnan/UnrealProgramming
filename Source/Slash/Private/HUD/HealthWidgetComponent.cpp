// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthWidgetComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthWidgetComponent::SetHealthPercent(float percent)
{
    if (_HealthBarWidget ==nullptr)
        _HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
    if (_HealthBarWidget && _HealthBarWidget->HealthBar)
        _HealthBarWidget->HealthBar->SetPercent(percent);
}
