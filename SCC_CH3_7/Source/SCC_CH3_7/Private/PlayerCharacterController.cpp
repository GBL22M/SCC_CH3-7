#include "PlayerCharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

APlayerCharacterController::APlayerCharacterController()
	:InputMappingContext(nullptr)
	,MoveAction(nullptr)
	,LookAction(nullptr)
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_PlayerCharacter(TEXT("/Game/Inputs/IMC_PlayerCharacter.IMC_PlayerCharacter"));
	if (IMC_PlayerCharacter.Succeeded())
	{
		InputMappingContext = IMC_PlayerCharacter.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Move(TEXT("/Game/Inputs/IA_Move.IA_Move"));
	if (IA_Move.Succeeded())
	{
		MoveAction = IA_Move.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Look(TEXT("/Game/Inputs/IA_Look.IA_Look"));
	if (IA_Look.Succeeded())
	{
		LookAction = IA_Look.Object;
	}

}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
