#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectGameInstance.generated.h"

UCLASS()
class GAMEPROJECT_API UProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Multiplayer")
	bool IsSteamSubsystemActive() const;
};
