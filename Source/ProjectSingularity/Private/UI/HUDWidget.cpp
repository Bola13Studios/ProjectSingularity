// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectSingularity/Public/UI/HUDWidget.h"

void UHUDWidget::BindToHealthComponent(UHealthComponent* InHealthComp)
{
	if (HealthWidget)
	{
		HealthWidget->BindToHealthComponent(InHealthComp);
	}
}
