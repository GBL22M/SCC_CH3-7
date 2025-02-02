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
	,RollSpeed(200.f)
	,UpSpeed(300.f)
	,Gravity(FVector(0.f, 0.f, -98.f))
	,MaxTraceDistance(20.f)
	,MaxCollisionSphereRadius(20.f)
	,OriginRotator(FRotator(0.f, 0.f, 0.f))
	,Velocity(FVector::ZeroVector)
	,CurrentRotation(FRotator::ZeroRotator)
	,IsMoveStart(false)
	,IsYawMoving(false)
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	SetRootComponent(CapsuleComp);

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("For Transform"));
	SceneComp->SetupAttachment(CapsuleComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshComp->SetupAttachment(SceneComp);

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

	TiltMoving();
}

void AFlyingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
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
			if (PlayerController->MoveAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->MoveAction
					, ETriggerEvent::Started
					, this
					, &AFlyingCharacter::StartMove);
			}
			if (PlayerController->MoveAction)	
			{
				EnhancedInputComp->BindAction(
					PlayerController->MoveAction
					, ETriggerEvent::Completed
					, this
					, &AFlyingCharacter::StopMove);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->LookAction
					, ETriggerEvent::Triggered
					, this
					, &AFlyingCharacter::Look);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->LookAction
					, ETriggerEvent::Completed
					, this
					, &AFlyingCharacter::StopLook);
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
	FVector NewLocation = FVector(MoveInput.X, MoveInput.Y, 0) * MoveSpeed * GetWorld()->GetDeltaSeconds();

	Velocity = NewLocation;

	if (IsMoveStart)
	{
		if (Velocity.X > 0.f)
		{
			TargetPitch = CurrentRotation.Pitch - 15.;
			DeltaPitch = Velocity.X;
			IsPositiveX = true;
		}
		if (Velocity.X < 0.f)
		{
			TargetPitch = CurrentRotation.Pitch + 15.;
			DeltaPitch = Velocity.X;
			IsPositiveX = false;
		}

		if (Velocity.Y > 0.f)
		{
			TargetRoll = CurrentRotation.Roll + 15.;
			DeltaRoll = Velocity.Y;
			IsPositiveY = true;
		}
		if (Velocity.Y < 0.f)
		{
			TargetRoll = CurrentRotation.Roll - 15.;
			DeltaRoll = Velocity.Y;
			IsPositiveY = false;
		}
	}

	IsMoveStart = false;
	AddActorLocalOffset(NewLocation);	
}

void AFlyingCharacter::StartMove(const FInputActionValue& value)
{
	CurrentRotation = GetActorRotation();
	OriginRotator = CurrentRotation;

	IsMoveStart = true;
}

void AFlyingCharacter::StopMove(const FInputActionValue& value)
{	
	Velocity = FVector(0.f, 0.f, 0.f);
}

void AFlyingCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();
	FRotator NewRotation = FRotator(LookInput.Y, LookInput.X, 0.f) * LookSpeed * GetWorld()->GetDeltaSeconds();

	AddActorLocalRotation(NewRotation);

	IsYawMoving = true;
}

void AFlyingCharacter::StopLook(const FInputActionValue& value)
{
	IsYawMoving = false;
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

	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_WorldStatic, Sphere);

	if (HasHit)					
		return true;	
	return false;
}

void AFlyingCharacter::TiltMoving()
{
	//[flying]
	//Tilt X(pitch)	
	if (IsPositiveX && CurrentRotation.Pitch >= TargetPitch)
	{		
		CurrentRotation.Pitch -= DeltaPitch * 10.f * GetWorld()->GetDeltaSeconds();		
		AddActorLocalRotation(FRotator(-DeltaPitch * 10.f * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
		//SceneComp->AddLocalRotation(FRotator(-DeltaPitch * 10.f * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
	}
	if (!IsPositiveX && CurrentRotation.Pitch <= TargetPitch)
	{
		CurrentRotation.Pitch += -DeltaPitch * 10.f * GetWorld()->GetDeltaSeconds();
		AddActorLocalRotation(FRotator(-DeltaPitch * 10.f * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
		//SceneComp->AddLocalRotation(FRotator(-DeltaPitch * 10.f * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
	}

	//Tilt Y(Roll)
	if (IsPositiveY && CurrentRotation.Roll <= TargetRoll)
	{
		CurrentRotation.Roll += DeltaRoll * 10.f * GetWorld()->GetDeltaSeconds();
		AddActorLocalRotation(FRotator(0.f, 0.f, DeltaRoll * 10.f * GetWorld()->GetDeltaSeconds()));
		//SceneComp->AddLocalRotation(FRotator(0.f, 0.f, DeltaRoll * 10.f * GetWorld()->GetDeltaSeconds()));
	}
	if (!IsPositiveY && CurrentRotation.Roll >= TargetRoll)
	{
		CurrentRotation.Roll += DeltaRoll * 10.f * GetWorld()->GetDeltaSeconds();
		AddActorLocalRotation(FRotator(0.f, 0.f, DeltaRoll * 10.f * GetWorld()->GetDeltaSeconds()));
		//SceneComp->AddLocalRotation(FRotator(0.f, 0.f, DeltaRoll * 10.f * GetWorld()->GetDeltaSeconds()));
	}

	//[stop]
	//back to origin rotator
	if(Velocity.Length() == 0.f)
	{		
		FRotator CurrentRotator = GetActorRotation();
		if (IsYawMoving)
		{
			OriginRotator = GetActorRotation();
			OriginRotator.Roll = 0.f;
		}
		FRotator ToOriginRotator = FMath::Lerp(CurrentRotator, OriginRotator, GetWorld()->GetDeltaSeconds() * 8.f);
		TargetPitch = 0.f;
		TargetRoll = 0.f;
		DeltaPitch = 0.f;
		DeltaRoll = 0.f;
		SetActorRotation(ToOriginRotator);
	}
}