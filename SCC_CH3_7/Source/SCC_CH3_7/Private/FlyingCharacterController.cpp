#include "FlyingCharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AFlyingCharacterController::AFlyingCharacterController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_PlayerCharacter(TEXT("/Game/Inputs/IMC_Fly.IMC_Fly"));
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
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Roll(TEXT("/Game/Inputs/IA_Roll.IA_Roll"));
	if (IA_Roll.Succeeded())
	{
		RollAction = IA_Roll.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_UpDown(TEXT("/Game/Inputs/IA_UpDown.IA_UpDown"));
	if (IA_UpDown.Succeeded())
	{
		UpDownAction = IA_UpDown.Object;
	}
}

void AFlyingCharacterController::BeginPlay()
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
