// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Components/StatsComponent.h"
#include "GameFramework/HUD.h"
#include "BasePlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API ABasePlayerHUD : public AHUD
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCommonActivatableWidget> PlayerHUDWidgetClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatsComponent> PlayerStatsComponent;
	
	
};
