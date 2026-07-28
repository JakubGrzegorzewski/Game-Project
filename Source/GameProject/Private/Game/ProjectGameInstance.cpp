#include "Game/ProjectGameInstance.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemNames.h"

bool UProjectGameInstance::IsSteamSubsystemActive() const
{
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
	return OnlineSubsystem != nullptr;
}
