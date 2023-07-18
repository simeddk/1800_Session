#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Widgets/IMenuInterface.h"
#include "CGameInstance.generated.h"

UCLASS()
class U05_SESSION_API UCGameInstance : public UGameInstance, public IIMenuInterface
{
	GENERATED_BODY()

public:
	UCGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	
public:
	UFUNCTION(BlueprintCallable, Exec)
		void LoadMenu();

	UFUNCTION(Exec)
		void Host() override;

	UFUNCTION(Exec)
		void Join(const FString& InAddress);

private:
	TSubclassOf<UUserWidget> MenuWidgetClass;

	class UCMenu* Menu;
};
