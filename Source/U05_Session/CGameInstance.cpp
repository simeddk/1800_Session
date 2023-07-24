#include "CGameInstance.h"
#include "Global.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CMenu.h"
#include "Widgets/CMenuBase.h"

const static FName SESSION_NAME = TEXT("MySession");

UCGameInstance::UCGameInstance(const FObjectInitializer& ObjectInitializer)
{
	CLog::Log("GameInstance::Constructor Called");

	CHelpers::GetClass(&MenuWidgetClass, "/Game/Widgets/WB_Menu");
	CHelpers::GetClass(&InGameWidgetClass, "/Game/Widgets/WB_InGame");
}

void UCGameInstance::Init()
{
	Super::Init();

	CLog::Log("GameInstance::Init Called");

	IOnlineSubsystem* oss = IOnlineSubsystem::Get();
	if (!!oss)
	{
		CLog::Log("OSS Name : " + oss->GetSubsystemName().ToString());

		//Session Event Binding
		SessionInterface = oss->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCGameInstance::OnFindSessionComplete);

			
			//Find Session
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid())
			{
				CLog::Log("Starting Find Session");

				SessionSearch->bIsLanQuery = true;
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}
		}
		
	}
	else
	{
		CLog::Log("OSS Not Found!");
	}

}

void UCGameInstance::LoadMenu()
{
	CheckNull(MenuWidgetClass);

	Menu = CreateWidget<UCMenu>(this, MenuWidgetClass);
	CheckNull(Menu);

	Menu->SetOwingGameInstance(this);
	Menu->Attach();
}

void UCGameInstance::LoadInGameMenu()
{
	CheckNull(InGameWidgetClass);

	UCMenuBase* inGameWidget = CreateWidget<UCMenuBase>(this, InGameWidgetClass);
	CheckNull(inGameWidget);

	inGameWidget->SetOwingGameInstance(this);
	inGameWidget->Attach();
}

void UCGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
	 	auto session = SessionInterface->GetNamedSession(SESSION_NAME);
		if (!!session)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UCGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bIsLANMatch = true;
		sessionSettings.NumPublicConnections = 5;
		sessionSettings.bShouldAdvertise = true;

		SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
	}
}

void UCGameInstance::Join(const FString& InAddress)
{
	CLog::Print("Join to " + InAddress);

	if (!!Menu)
		Menu->Detach();

	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);
	controller->ClientTravel(InAddress, ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::ReturnToMainMenu()
{
	APlayerController* controller = GetFirstLocalPlayerController();
	CheckNull(controller);
	controller->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::OnCreateSessionComplete(FName InSessionName, bool InSuccess)
{
	//���� ���� ����
	if (InSuccess == false)
	{
		CLog::Log("Could not create Session!!");
		return;
	}

	//���� ���� ����
	CLog::Log("Session Name : " + InSessionName.ToString());

	if (!!Menu)
		Menu->Detach();

	CLog::Print("Host");

	UWorld* world = GetWorld();
	CheckNull(world);
	world->ServerTravel("/Game/Maps/Play?listen");
}

void UCGameInstance::OnDestroySessionComplete(FName InSessionName, bool InSuccess)
{
	if (InSuccess == true)
		CreateSession();
}

void UCGameInstance::OnFindSessionComplete(bool InSuccess)
{
	if (InSuccess == true && SessionSearch.IsValid())
	{
		CLog::Log("Finished Find Sessoin");

		CLog::Log("==========<Find Session Results>==========");
		for (const auto& searchResult : SessionSearch->SearchResults)
		{
			CLog::Log(" -> Session ID : " + searchResult.GetSessionIdStr());
			CLog::Log(" -> Ping : " + FString::FromInt(searchResult.PingInMs));
		}
		CLog::Log("===========================================");

	}
}

