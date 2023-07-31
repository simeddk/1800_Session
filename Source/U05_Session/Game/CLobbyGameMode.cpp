#include "CLobbyGameMode.h"
#include "Global.h"

void ACLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= 3)
	{
		UWorld* world = GetWorld();
		CheckNull(world);
		world->ServerTravel("/Game/Maps/Play?listen");
	}
}

void ACLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;

}