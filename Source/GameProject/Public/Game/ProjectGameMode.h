#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectGameMode.generated.h"

UCLASS()
class GAMEPROJECT_API AProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplayer")
	int32 MaxPlayers = 4;

protected:
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
};
