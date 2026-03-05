// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectSingularity/Public/UI/HealthWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthWidget::BindToHealthComponent(UHealthComponent* InHealthComp)
{
	if (HealthComp)
	{
		HealthComp->OnHealthChanged.RemoveDynamic(this, &UHealthWidget::HandleHealthChanged);
	}

	HealthComp = InHealthComp;

	if (HealthComp)
	{
		HealthComp->OnHealthChanged.AddDynamic(this, &UHealthWidget::HandleHealthChanged);
		RefreshUI(HealthComp->GetHealth(), HealthComp->GetMaxHealth());
	}
}

void UHealthWidget::NativeDestruct()
{
	if (HealthComp)
	{
		HealthComp->OnHealthChanged.RemoveDynamic(this, &UHealthWidget::HandleHealthChanged);
	}

	Super::NativeDestruct();
}

void UHealthWidget::HandleHealthChanged(float Current, float Max, float Delta, AActor* InstigatorActor)
{
	RefreshUI(Current, Max);
}

void UHealthWidget::RefreshUI(float Current, float Max)
{
	const float Percent = (Max > 0.0f) ? (Current / Max) : 0.0f;

	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}

	if (HealthText)
	{
		const int32 C = FMath::RoundToInt(Current);
		const int32 M = FMath::RoundToInt(Max);
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), C, M)));
	}
}
