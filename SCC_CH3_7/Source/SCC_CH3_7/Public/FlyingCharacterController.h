#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FlyingCharacterController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SCC_CH3_7_API AFlyingCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	AFlyingCharacterController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> RollAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> UpDownAction;

protected:
	virtual void BeginPlay() override;
};
