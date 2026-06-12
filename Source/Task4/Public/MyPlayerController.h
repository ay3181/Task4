#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class AMyPawn;
class AHand;

UCLASS()
class TASK4_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FlyAction;
	//여기부터 플레이어 캐릭터 변경 (할 줄 몰라서 AI 도움 받음...)
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ChangeAction;

	void ChangePawn();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possess")
	AMyPawn* FirstPawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possess")
	AHand* SecondPawn;

private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};
