#include "CGameInstance.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"

UCGameInstance::UCGameInstance(const FObjectInitializer& ObjectInitializer)
{
	CLog::Log("GameInstance::Constructor Called");

	TSubclassOf<UUserWidget> menuWidgetClass;
	CHelpers::GetClass(&menuWidgetClass, "/Game/Widgets/WB_Menu");
	CLog::Log(menuWidgetClass->GetName());
	
}

void UCGameInstance::Init()
{
	Super::Init();

	CLog::Log("GameInstance::Init Called");

}

void UCGameInstance::Host()
{
	CLog::Print("Host");
	
	UWorld* world = GetWorld();
	CheckNull(world);

	world->ServerTravel("/Game/Maps/Play?listen");
}

void UCGameInstance::Join(const FString& InAddress)
{
	CLog::Print("Join to " + InAddress);

	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);
	controller->ClientTravel(InAddress, ETravelType::TRAVEL_Absolute);
}

