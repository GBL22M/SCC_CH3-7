#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerCharacter.generated.h"

class UCapsuleComponent;
class USkeletaMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class SCC_CH3_7_API APlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<UCapsuleComponent> CapsuleComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<UCameraComponent> CameraComp;

private:
	float MoveSpeed;
	float LookSpeed;
};
