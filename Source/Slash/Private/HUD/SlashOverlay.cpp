// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USlashOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void USlashOverlay::SetStaminaBarPercent(float Percent)
{
	if (HealthStaminaBar)
	{
		HealthStaminaBar->SetPercent(Percent);
	}
}

void USlashOverlay::SetGold(int32 Gold)
{
	if (GoldText)
	{
		GoldText->SetText(getFTextFromInt(Gold));
	}
}

void USlashOverlay::SetSouls(int32 Souls)
{
	if (SoulText)
	{
		
		SoulText->SetText(getFTextFromInt(Souls));
	}
}

FText USlashOverlay::getFTextFromInt(int32 inp)
{
	const FString String = FString::Printf(TEXT("%d"), inp);
	const FText Text = FText::FromString(String);
	return Text;
}
