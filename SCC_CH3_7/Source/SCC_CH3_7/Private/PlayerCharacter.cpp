#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

APlayerCharacter::APlayerCharacter()
	:MoveSpeed(500.f)
	,LookSpeed(300.f)
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));	
	SetRootComponent(CapsuleComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	
	//skeletal mesh position setting
	SkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	SkeletalMeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	//skeletal mesh asset setting
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeletalMeshAsset(TEXT("/Game/MonsterForSurvivalGame/Mesh/PBR/Mushroom_SK.Mushroom_SK"));
	if (SkeletalMeshAsset.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMeshAsset(SkeletalMeshAsset.Object);
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(CapsuleComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	//disable simulate physics
	CapsuleComp->SetSimulatePhysics(false);
	SkeletalMeshComp->SetSimulatePhysics(false);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(GetController()))
		{
			//Action binding
			if (PlayerController->MoveAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->MoveAction
					, ETriggerEvent::Triggered
					, this
					, &APlayerCharacter::Move);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->LookAction
					, ETriggerEvent::Triggered
					, this
					, &APlayerCharacter::Look);
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	const FVector2D MoveInput = value.Get<FVector2D>();
	FVector NewLocation = FVector(MoveInput.X, MoveInput.Y, 0.f) * MoveSpeed * GetWorld()->GetDeltaSeconds();
	
	AddActorLocalOffset(NewLocation);
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();
	FRotator NewRotation = FRotator(0.f, LookInput.X, 0.f) * LookSpeed * GetWorld()->GetDeltaSeconds();
	
	AddActorLocalRotation(NewRotation);

	SpringArmComp->AddLocalRotation(FRotator(LookInput.Y, 0.f, 0.f));
}