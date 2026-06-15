#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyPawn.h"
#include "Hand.h"

AMyPlayerController::AMyPlayerController()
	: InputMappingContext(nullptr), MoveAction(nullptr), LookAction(nullptr), FlyAction(nullptr)
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	//이 아래로 쭉 플레이어 캐릭터 변경 (할 줄 몰라서 AI 도움 받음...22)
	if (FirstPawn)
	{
		Possess(FirstPawn);
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ChangeAction)
		{
			EnhancedInput->BindAction(ChangeAction, ETriggerEvent::Started, this, &AMyPlayerController::ChangePawn);
		}
	}
}

void AMyPlayerController::ChangePawn()
{
	APawn* CurrentPawn = GetPawn();
	if (!CurrentPawn) return;

	APawn* TargetPawn = nullptr;

	if (CurrentPawn == FirstPawn)
	{
		TargetPawn = SecondPawn;
	}
	else if (CurrentPawn == SecondPawn)
	{
		TargetPawn = FirstPawn;
	}

	if (TargetPawn)
	{
		UnPossess();
		Possess(TargetPawn);
	}
}