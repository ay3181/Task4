#include "MyPawn.h"
#include "MyPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

AMyPawn::AMyPawn()
{
	//Ä¸½¶ ÄÄÆ÷³ÍÆ®
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComponent);

	//½ºÄÌ·¹Å» ¸Å½Ã
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Game/Fab/GameDev_Starter_Kit_-_Platformer__Free_Edition_/Character_Dev.Character_Dev"));

	if (SkeletalMesh.Succeeded())
	{
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh.Object);
		SkeletalMeshComponent->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	}

	//½ºÇÁ¸µ¾Ï
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(CapsuleComponent);
	SpringArmComponent->TargetArmLength = 200.0f;
	SpringArmComponent->bUsePawnControlRotation = false;

	//Ä«¸Þ¶ó
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	NormalSpeed = 600.0f;

}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
					&AMyPawn::Move);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Look);
			}
		}
	}

}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPawn::Tick(float DeltaTime)
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

void AMyPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	MoveInput = value.Get<FVector2D>();
	
}

void AMyPawn::Look(const FInputActionValue& value)
{
	LookInput = value.Get<FVector2D>();
}