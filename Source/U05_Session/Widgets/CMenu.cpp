#include "CMenu.h"
#include "Global.h"
#include "Components/Button.h"

bool UCMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	CheckFalseResult(bSuccess, false);

	CheckNullResult(HostButton, false);
	HostButton->OnClicked.AddDynamic(this, &UCMenu::HostServer);

	return true;
}

void UCMenu::SetOwingGameInstance(IIMenuInterface* InOwingInterface)
{
	OwingGameInstance = InOwingInterface;
}

void UCMenu::Attach()
{
	AddToViewport();

	bIsFocusable = true;

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	UWorld* world = GetWorld();
	CheckNull(world);
	APlayerController* controller = world->GetFirstPlayerController();
	CheckNull(controller);

	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = true;
}

void UCMenu::Detach()
{
	RemoveFromParent();

	bIsFocusable = false;

	FInputModeGameOnly inputMode;

	UWorld* world = GetWorld();
	CheckNull(world);
	APlayerController* controller = world->GetFirstPlayerController();
	CheckNull(controller);

	controller->SetInputMode(inputMode);
	controller->bShowMouseCursor = false;
}

void UCMenu::HostServer()
{
	CheckNull(OwingGameInstance);

	OwingGameInstance->Host();
}

