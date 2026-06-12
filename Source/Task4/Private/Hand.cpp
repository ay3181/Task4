#include "Hand.h"
#include "MyPlayerController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

AHand::AHand()
{
	//박스컴포넌트
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	//스태틱 메시
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(BoxComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Game/Fab/Mano_Hand_Cyborg/mano_hand_cyborg.mano_hand_cyborg"));
	if (StaticMesh.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh.Object);
		StaticMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
		StaticMeshComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	}

	//스프링 암
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(BoxComponent);
	SpringArmComponent->TargetArmLength = 200.0f;
	SpringArmComponent->bUsePawnControlRotation = false;

	//카메라
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	NormalSpeed = 600.0f;

}

void AHand::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLocation = GetActorLocation();
	ForwardVector = GetActorForwardVector();
	RightVector = GetActorRightVector();

	if (!MoveInput.IsNearlyZero())
	{
		MoveVector = (ForwardVector * MoveInput.X) + (RightVector * MoveInput.Y);
		MoveVector.Z = 0.0f;
		MoveVector.Normalize();
		SetActorLocation(CurrentLocation + (MoveVector * NormalSpeed * DeltaTime));
		MoveInput = FVector2D::ZeroVector;
	}
	
	if (!FMath::IsNearlyZero(FlyInput))
	{
		if (FlyInput > 0.0f)
		{
			SetActorLocation(CurrentLocation + GetActorUpVector() * NormalSpeed * DeltaTime);
		}
		else
		{
			SetActorLocation(CurrentLocation - GetActorUpVector() * NormalSpeed * DeltaTime);
		}
		FlyInput = 0.0f;
	}

	if (!FMath::IsNearlyZero(LookInput.X))
	{
		AddActorLocalRotation(FRotator(0.0f, LookInput.X, 0.0f));
	}
	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		SpringArmComponent->AddRelativeRotation(FRotator(-LookInput.Y, 0.0f, 0.0f));
	}
	LookInput = FVector2D::ZeroVector;
}

void AHand::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AHand::Move);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AHand::Look);
			}

			if (PlayerController->FlyAction)
			{
				EnhancedInput->BindAction(
					PlayerController->FlyAction,
					ETriggerEvent::Triggered,
					this,
					&AHand::Fly);
			}
		}
	}

}

void AHand::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	MoveInput = value.Get<FVector2D>();

}

void AHand::Look(const FInputActionValue& value)
{
	LookInput = value.Get<FVector2D>();
}

void AHand::Fly(const FInputActionValue& value)
{
	FlyInput = value.Get<float>();
}