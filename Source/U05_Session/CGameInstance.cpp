#include "CGameInstance.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CMenu.h"

UCGameInstance::UCGameInstance(const FObjectInitializer& ObjectInitializer)
{
	CLog::Log("GameInstance::Constructor Called");

	CHelpers::GetClass(&MenuWidgetClass, "/Game/Widgets/WB_Menu");
}

void UCGameInstance::Init()
{
	Super::Init();

	CLog::Log("GameInstance::Init Called");

}

void UCGameInstance::LoadMenu()
{
	CheckNull(MenuWidgetClass);

	Menu = CreateWidget<UCMenu>(this, MenuWidgetClass);
	CheckNull(Menu);

	Menu->SetOwingGameInstance(this);
	Menu->Attach();
}

void UCGameInstance::Host()
{
	if (!!Menu)
		Menu->Detach();

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

