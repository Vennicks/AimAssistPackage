// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AimAssistPackageCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AAimAssistPackageCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;


public:
	UPROPERTY(EditAnywhere, Category = "Aim Assist Settings | Targetting")
		TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, Category = "Aim Assist Settings | Targetting")
		TArray<TEnumAsByte<EObjectTypeQuery>> ActorsTypeToAim;

	UPROPERTY(EditAnywhere, Category = "Aim Assist Settings")
		float Sensibility = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Aim Assist Settings")
		float SpeedRotationThreshold = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Aim Assist Settings")
		float MaxRange = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Aim Assist Settings")
		float CapsuleRadius = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Aim Assist Settings")
		float CapsuleHalfHeight = 25.0f;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UFUNCTION(BlueprintCallable, Category = "Aim assist Utility")
	bool Detection(FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Aim assist Utility")
	void AutoRotate(FHitResult hit);

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	bool IsAimAssistEnable(float DeltaTime);

	virtual void BeginPlay();
	virtual void Tick(float v);
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;



public:
	AAimAssistPackageCharacter();
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


private:
	FHitResult hit;
	FVector PreviousRotation = FVector::Zero();
};

