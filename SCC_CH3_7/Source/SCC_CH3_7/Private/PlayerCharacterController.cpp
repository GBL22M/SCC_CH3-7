#include "PlayerCharacterController.h"
#include "EnhancedInputSubsystems.h"

APlayerCharacterController::APlayerCharacterController()
	:InputMappingContext(nullptr)
	,MoveAction(nullptr)
	,LookAction(nullptr)
{

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
