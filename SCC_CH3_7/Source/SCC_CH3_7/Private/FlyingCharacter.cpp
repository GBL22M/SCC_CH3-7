#include "FlyingCharacter.h"
#include "FlyingCharacterController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

AFlyingCharacter::AFlyingCharacter()
	:MoveSpeed(500.f)
	,LookSpeed(300.f)
	,RollSpeed(300.f)
	,UpSpeed(300.f)
	,Gravity(FVector(0.f, 0.f, -98.f))
	,MaxTraceDistance(20.f)
	,MaxCollisionSphereRadius(20.f)
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	SetRootComponent(CapsuleComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);

	//skeletal mesh position setting
	SkeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -48.f));
	SkeletalMeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	//skeletal mesh asset setting
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeletalMeshAsset(TEXT("/Game/MonsterForSurvivalGame/Mesh/PBR/Swarm08_SK.Swarm08_SK"));
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

void AFlyingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFlyingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsGround())
	{

	}
	if (!IsGround())
	{
		FVector NewLocation = Gravity * DeltaTime;
		AddActorWorldOffset(NewLocation, true);
	}
}

void AFlyingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AFlyingCharacterController* PlayerController = Cast<AFlyingCharacterController>(GetController()))
		{
			//Action binding
			if (PlayerController->MoveAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->MoveAction
					, ETriggerEvent::Triggered
					, this
					, &AFlyingCharacter::Move);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->LookAction
					, ETriggerEvent::Triggered
					, this
					, &AFlyingCharacter::Look);
			}

			if (PlayerController->UpDownAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->UpDownAction
					, ETriggerEvent::Triggered
					, this
					, &AFlyingCharacter::UpDown);
			}

			if (PlayerController->RollAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->RollAction
					, ETriggerEvent::Triggered
					, this
					, &AFlyingCharacter::Roll);
			}
		}
	}
}

void AFlyingCharacter::Move(const FInputActionValue& value)
{
	const FVector2D MoveInput = value.Get<FVector2D>();
	FVector NewLocation = FVector(MoveInput.X, MoveInput.Y, 0.f) * MoveSpeed * GetWorld()->GetDeltaSeconds();

	AddActorLocalOffset(NewLocation);
}

void AFlyingCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();
	FRotator NewRotation = FRotator(LookInput.Y, LookInput.X, 0.f) * LookSpeed * GetWorld()->GetDeltaSeconds();

	AddActorLocalRotation(NewRotation);
}

void AFlyingCharacter::UpDown(const FInputActionValue& value)
{
	float InputValue = value.Get<float>();
	FVector NewLocation = FVector::UpVector * InputValue * UpSpeed * GetWorld()->GetDeltaSeconds();
		
	AddActorWorldOffset(NewLocation);	
}

void AFlyingCharacter::Roll(const FInputActionValue& value)
{
	float InputValue = value.Get<float>();

	FRotator NewRotation = FRotator(0.f, 0.f, InputValue * RollSpeed * GetWorld()->GetDeltaSeconds());
	AddActorLocalRotation(NewRotation);
}

bool AFlyingCharacter::IsGround()
{
	FVector Start = GetActorLocation();
	FVector End = Start + FVector::DownVector * MaxTraceDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);


	FCollisionShape Sphere = FCollisionShape::MakeSphere(MaxCollisionSphereRadius);
	DrawDebugSphere(GetWorld(),	End, MaxCollisionSphereRadius, 20, FColor::Blue);

	FHitResult HitResult;

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start, End,
		FQuat::Identity,
		ECC_WorldStatic,
		Sphere
	);

	if (HasHit)					
		return true;
	
	return false;
}
