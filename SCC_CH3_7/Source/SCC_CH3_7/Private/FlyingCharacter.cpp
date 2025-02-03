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
	,Velocity(FVector::ZeroVector)
	,OriginRotator(FRotator(0.f, 0.f, 0.f))
	,CurrentRotation(FRotator::ZeroRotator)
	,IsPositiveX(0)
	,IsPositiveY(0)
	,IsMoveStart(false)
	,IsYawMoving(false)
	,IsRollMoving(false)
	,ResetSpeed(8.f)
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

void AFlyingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//reset position
	if (IsGround())
	{
		FRotator NowRotator = GetActorRotation();
		FRotator ToOriginRotator = FMath::Lerp(NowRotator, FRotator(0.f, NowRotator.Yaw, 0.f), GetWorld()->GetDeltaSeconds() * 10.f);

		SetActorRotation(ToOriginRotator);

		ResetRotation(true);

		SetActorRelativeRotation(FRotator(0.f, CurrentRotation.Yaw, CurrentRotation.Roll));
	}

	//apply gravity
	if (!IsGround())
	{				
		AddActorWorldOffset(Gravity * DeltaTime, true);
	}	

	//Tilt
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
			if (PlayerController->RollAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->RollAction
					, ETriggerEvent::Started
					, this
					, &AFlyingCharacter::StartRoll);
			}
			if (PlayerController->RollAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->RollAction
					, ETriggerEvent::Completed
					, this
					, &AFlyingCharacter::StopRoll);
			}
			if (PlayerController->ResetToZeroAction)
			{
				EnhancedInputComp->BindAction(
					PlayerController->ResetToZeroAction
					, ETriggerEvent::Triggered
					, this
					, &AFlyingCharacter::ResetToZero);
			}
		}
	}
}

void AFlyingCharacter::Move(const FInputActionValue& value)
{
	const FVector2D MoveInput = value.Get<FVector2D>();	
	FVector NewLocation = FVector(MoveInput.X, MoveInput.Y, 0) * MoveSpeed * GetWorld()->GetDeltaSeconds();
	Velocity = NewLocation;

	//if move start make tilting target rotation
	if (IsMoveStart)
	{
		if (Velocity.X > 0.f)
		{
			TargetPitch = CurrentRotation.Pitch - 15.;
			DeltaPitch = Velocity.X;
			IsPositiveX = 1;
		}
		if (Velocity.X < 0.f)
		{
			TargetPitch = CurrentRotation.Pitch + 15.;
			DeltaPitch = Velocity.X;
			IsPositiveX = 2;
		}

		if (Velocity.Y > 0.f)
		{
			TargetRoll = CurrentRotation.Roll + 15.;
			DeltaRoll = Velocity.Y;
			IsPositiveY = 1;
		}
		if (Velocity.Y < 0.f)
		{
			TargetRoll = CurrentRotation.Roll - 15.;
			DeltaRoll = Velocity.Y;
			IsPositiveY = 2;
		}
	}

	IsMoveStart = false;
	AddActorLocalOffset(NewLocation);	
}

void AFlyingCharacter::StartMove(const FInputActionValue& value)
{
	//save start position
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

void AFlyingCharacter::StartRoll(const FInputActionValue& value)
{
	IsRollMoving = true;
}

void AFlyingCharacter::StopRoll(const FInputActionValue& value)
{
	IsRollMoving = false;
	ResetRotation(true);
}

void AFlyingCharacter::ResetToZero(const FInputActionValue& value)
{
	//reset rotation (pitch and roll to zero)
	if (value.Get<bool>())
	{
		FRotator NowRotator = GetActorRotation();
		FRotator ToOriginRotator = FMath::Lerp(NowRotator, FRotator(0.f, NowRotator.Yaw, 0.f), GetWorld()->GetDeltaSeconds() * ResetSpeed);

		SetActorRotation(ToOriginRotator);
		
		ResetRotation(true);
	}
}

bool AFlyingCharacter::IsGround()
{
	FVector Start = GetActorLocation();
	FVector End = Start + FVector::DownVector * MaxTraceDistance;
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(MaxCollisionSphereRadius);
	//DrawDebugSphere(GetWorld(),	End, MaxCollisionSphereRadius, 20, FColor::Blue);

	FHitResult HitResult;

	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_WorldStatic, Sphere);

	if (HasHit)					
		return true;	
	return false;
}

void AFlyingCharacter::TiltMoving()
{
	//[reset]
	//yaw moving or roll moving -> stop tilting (remove tilting values)
	if (IsYawMoving || IsRollMoving)
	{
		ResetRotation(true);
		return;
	}

	//[stop]
	//back to origin rotator
	if(Velocity.Length() == 0.f)
	{		
		FRotator NowRotator = GetActorRotation();
		FRotator ToOriginRotator = FMath::Lerp(NowRotator, OriginRotator, GetWorld()->GetDeltaSeconds() * ResetSpeed);

		SetActorRotation(ToOriginRotator);		
		IsPositiveX = 0;
		IsPositiveY = 0;
	}

	//[flying]
	//Tilt X(pitch)	
	if (IsPositiveX == 1 && CurrentRotation.Pitch >= TargetPitch)
	{		
		CurrentRotation.Pitch -= DeltaPitch * 10.f * GetWorld()->GetDeltaSeconds();		
		AddActorLocalRotation(FRotator(-DeltaPitch * 10.f * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
	}
	if (IsPositiveX == 2 && CurrentRotation.Pitch <= TargetPitch)
	{
		CurrentRotation.Pitch += -DeltaPitch * 10.f * GetWorld()->GetDeltaSeconds();
		AddActorLocalRotation(FRotator(-DeltaPitch * 10.f * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
	}

	//Tilt Y(Roll)
	if (IsPositiveY == 1 && CurrentRotation.Roll <= TargetRoll)
	{
		CurrentRotation.Roll += DeltaRoll * 10.f * GetWorld()->GetDeltaSeconds();
		AddActorLocalRotation(FRotator(0.f, 0.f, DeltaRoll * 10.f * GetWorld()->GetDeltaSeconds()));
	}
	if (IsPositiveY == 2 && CurrentRotation.Roll >= TargetRoll)
	{
		CurrentRotation.Roll += DeltaRoll * 10.f * GetWorld()->GetDeltaSeconds();
		AddActorLocalRotation(FRotator(0.f, 0.f, DeltaRoll * 10.f * GetWorld()->GetDeltaSeconds()));
	}
}

void AFlyingCharacter::ResetRotation(bool ResetOrigin)
{
	if (ResetOrigin)
	{
		CurrentRotation = GetActorRotation();
		OriginRotator = CurrentRotation;
	}	

	IsPositiveX = 0;
	IsPositiveY = 0;
	TargetPitch = 0.f;
	TargetRoll = 0.f;
	DeltaPitch = 0.f;
	DeltaRoll = 0.f;
}
