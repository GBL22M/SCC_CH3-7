#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerCharacter.generated.h"

class UCapsuleComponent;
class USkeletaMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SCC_CH3_7_API APlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY()
	TObjectPtr<UCapsuleComponent> CapsuleComp;
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;
	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComp;

};
