// Copyright Epic Games, Inc. All Rights Reserved.

#include "AimAssistPackageCharacter.h"
#include "AimAssistPackageProjectile.h"
#include "Enemy.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GM_Game.h"
#include "TP_WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


AAimAssistPackageCharacter::AAimAssistPackageCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}


void AAimAssistPackageCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	CurrentHealth = HealthPointMax;
	ActorsToIgnore.Add(this);
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
	PlayerStartLocation = FoundActors[0]->GetActorLocation();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AAimAssistPackageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetHasRifle() && IsAimAssistEnable(DeltaTime) && Detection(hit))
		AutoRotate(hit);
}

#pragma region Inputs Handling
void AAimAssistPackageCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAimAssistPackageCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAimAssistPackageCharacter::Look);
	}
}

void AAimAssistPackageCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AAimAssistPackageCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

#pragma endregion

#pragma region Rifle Handling
void AAimAssistPackageCharacter::SetHasRifle(bool bNewHasRifle, FString RifleName)
{
	bHasRifle = bNewHasRifle;
	rifleName = RifleName;
}

bool AAimAssistPackageCharacter::GetHasRifle()
{
	return bHasRifle;
}	

void AAimAssistPackageCharacter::DropRifle()
{
	TArray<USceneComponent*> Components = Mesh1P->GetAttachChildren();
	for (auto component : Components)
	{
		component->DestroyComponent();
	}
	
	if (WeaponToSpawn)
		GetWorld()->SpawnActor<AActor>(WeaponToSpawn, GetTransform());
	
}

#pragma endregion

#pragma region Aim Assist
bool AAimAssistPackageCharacter::Detection(FHitResult &Hit)
{
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = FirstPersonCameraComponent->GetForwardVector() * MaxRange + Start;
	
	UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), Start, End, CapsuleRadius, CapsuleHalfHeight, ActorsTypeToAim, true, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);
	
	return Hit.bBlockingHit;
}

void AAimAssistPackageCharacter::AutoRotate(FHitResult Hit)
{
	FVector HitTargetRelativeLocation = SelectComponent(Hit)->GetComponentLocation() - Hit.ImpactPoint;
	HitTargetRelativeLocation.Normalize(0.0001);
	FRotator TransformToRotation = UKismetMathLibrary::Conv_VectorToRotator(HitTargetRelativeLocation);
	FRotator NewRotation = UKismetMathLibrary::RInterpTo(GetController()->GetControlRotation(), TransformToRotation, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), Sensibility);

	GetController()->SetControlRotation(NewRotation);
}

bool AAimAssistPackageCharacter::IsAimAssistEnable(float DeltaTime)
{
	// Calculate the rotation speed based on the change in orientation over time
	FVector CurrentRotation = GetActorRotation().Euler();
	FVector deltaRotation = (CurrentRotation - PreviousRotation) / DeltaTime;
	float rotationSpeed = deltaRotation.Size();
	
	PreviousRotation = CurrentRotation;
	bool bRotationSpeedExceedsLimit = FMath::Abs(rotationSpeed) > SpeedRotationThreshold;

	return bRotationSpeedExceedsLimit;
}

UPrimitiveComponent* AAimAssistPackageCharacter::SelectComponent(FHitResult Hit)
{
	if (SelectedComponent == nullptr || SelectedComponent == Hit.GetComponent())
	{
		SelectedComponent = Hit.GetComponent();
		return SelectedComponent;
	}

	float SelectedRelativeLength = (SelectedComponent->GetComponentLocation() - Hit.ImpactPoint).Size();
	float NewComponentRelativeLength = (Hit.GetComponent()->GetComponentLocation() - Hit.ImpactPoint).Size();

	if (SelectedComponent->GetName() == HighPriorityComponent)
		SelectedRelativeLength -= ComponentSwapTreshold;

	else if (Hit.GetComponent()->GetName() == HighPriorityComponent)
		NewComponentRelativeLength -= ComponentSwapTreshold;

	if (NewComponentRelativeLength < SelectedRelativeLength)
		SelectedComponent = Hit.GetComponent();

	return SelectedComponent;
}
#pragma endregion

float AAimAssistPackageCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,	AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0)
		KillPlayer();
	return CurrentHealth;
}

void AAimAssistPackageCharacter::KillPlayer()
{
	// Get the current level name
	const FString& CurrentLevelName = GetWorld()->GetMapName();

	// Reopen the current level
	UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName), true);
}