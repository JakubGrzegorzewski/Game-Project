// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ProjectSettings.generated.h"

class UCommonActivatableWidget;
/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Project Gameplay Settings"))
class GAMEPROJECT_API UProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

};
