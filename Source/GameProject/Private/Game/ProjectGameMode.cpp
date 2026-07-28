#include "Game/ProjectGameMode.h"

#include "Characters/Player/BasePlayerController.h"
#include "Characters/Player/BasePlayerState.h"
#include "GameFramework/GameStateBase.h"

AProjectGameMode::AProjectGameMode()
{
	PlayerControllerClass = ABasePlayerController::StaticClass();
	PlayerStateClass = ABasePlayerState::StaticClass();
}

void AProjectGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	if (!ErrorMessage.IsEmpty())
	{
		return;
	}

	const int32 CurrentPlayerCount = GameState ? GameState->PlayerArray.Num() : 0;
	if (CurrentPlayerCount >= MaxPlayers)
	{
		ErrorMessage = TEXT("Session is full.");
	}
}
