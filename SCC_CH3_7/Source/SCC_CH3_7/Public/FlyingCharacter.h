#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlyingCharacter.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActgionValue;

UCLASS()
class SCC_CH3_7_API AFlyingCharacter : public APawn
{
	GENERATED_BODY()

public:
	AFlyingCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//bind function
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void UpDown(const FInputActionValue& value);
	UFUNCTION()
	void Roll(const FInputActionValue& value);

	//
	UFUNCTION()
	bool IsGround();

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
	float RollSpeed;
	float UpSpeed;

	FVector Gravity;

	float MaxTraceDistance;
	float MaxCollisionSphereRadius;
};
