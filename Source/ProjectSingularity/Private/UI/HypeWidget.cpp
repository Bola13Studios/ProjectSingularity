// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectSingularity/Public/UI/HypeWidget.h"

#include "Components/TextBlock.h"
#include "Components/Hype/HypeComponent.h"

void UHypeWidget::BindToHypeComponent(UHypeComponent* InHypeComp)
{
	if (HypeComp == InHypeComp)
	{
		return;
	}

	if (HypeComp)
	{
		HypeComp->onHypeChanged.RemoveDynamic(this, &UHypeWidget::HandleHypeChanged);
	}

	HypeComp = InHypeComp;

	if (HypeComp)
	{
		HypeComp->onHypeChanged.AddDynamic(this, &UHypeWidget::HandleHypeChanged);
		RefreshUI(HypeComp->GetHype());
	}
}

void UHypeWidget::NativeDestruct()
{
	if (HypeComp)
	{
		HypeComp->onHypeChanged.RemoveDynamic(this, &UHypeWidget::HandleHypeChanged);
	}

	Super::NativeDestruct();
}

void UHypeWidget::HandleHypeChanged(int CurrentHype, int Delta)
{
	RefreshUI(CurrentHype);
}

void UHypeWidget::RefreshUI(int CurrentHype)
{
	if (HypeText)
	{
		HypeText->SetText(FText::AsNumber(CurrentHype));
	}
}