#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlyingCharacter.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
struct FInputActgionValue;

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SCC_CH3_7_API AFlyingCharacter : public APawn
{
	GENERATED_BODY()

public:
	AFlyingCharacter();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//bind function
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartMove(const FInputActionValue& value);
	UFUNCTION()
	void StopMove(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StopLook(const FInputActionValue& value);
	UFUNCTION()
	void UpDown(const FInputActionValue& value);
	UFUNCTION()
	void Roll(const FInputActionValue& value);
	UFUNCTION()
	void StartRoll(const FInputActionValue& value);
	UFUNCTION()
	void StopRoll(const FInputActionValue& value);
	UFUNCTION()
	void ResetToZero(const FInputActionValue& value);

	//
	UFUNCTION()
	bool IsGround();
	UFUNCTION()
	void TiltMoving();
	UFUNCTION()
	void ResetRotation(bool ResetOrigin);
	UFUNCTION()
	void CollisionCheck();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<UCapsuleComponent> CapsuleComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Components")
	TObjectPtr<UNiagaraComponent> NiagaraComp1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Components")	
	TObjectPtr<UNiagaraComponent> NiagaraComp2;

private:
	float MoveSpeed;
	float LookSpeed;
	float RollSpeed;
	float UpSpeed;

	FVector Gravity;

	float MaxTraceDistance;
	float MaxCollisionSphereRadius;

	FVector Velocity;
	FRotator OriginRotator;
	FRotator CurrentRotation;
	
	double TargetPitch;
	double TargetRoll;
	double DeltaPitch;
	double DeltaRoll;

	int IsPositiveX;
	int IsPositiveY;

	bool IsMoveStart;
	bool IsYawMoving;
	bool IsRollMoving;

	float ResetSpeed;
};
