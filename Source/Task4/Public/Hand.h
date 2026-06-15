#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Hand.generated.h"

class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class TASK4_API AHand : public APawn
{
	GENERATED_BODY()

public:
	AHand();
	UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	FVector2D MoveInput;
	FVector2D LookInput;
	float NormalSpeed;
	float FlyInput;

	FVector CurrentLocation, MoveVector;
	FVector ForwardVector, RightVector;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void Fly(const FInputActionValue& value);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
