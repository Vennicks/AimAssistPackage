// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class AIMASSISTPACKAGE_API AEnemy : public ACharacter
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		USkeletalMeshComponent* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		USceneComponent* ShootingPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Zone")
		UCapsuleComponent* HeadZone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Zone")
		UCapsuleComponent* BodyZone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Zone")
		UCapsuleComponent* LegZone;
		UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0, UIMax = 100), Category = "Weapon")
		int AimPercentage = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0, UIMax = 100), Category = "Weapon")
		float MissingOffset = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
		float Damages = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
		float HealthPointMax = 100;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Handling | Weak Zone")
		float WeakZoneMultiplier = 1.3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Handling | Weak Zone")
		FString WeakZoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Handling | Mid Zone")
		float MidZoneMultiplier = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Handling | Mid Zone")
		FString MidZoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Handling | Resistant Zone")
		float ResistantZoneMultiplier = 0.7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Handling | Resistant Zone")
		FString ResistantZoneName;



	AEnemy();

	UFUNCTION(BlueprintCallable)
		void Firing();

	UFUNCTION(BlueprintCallable)
		float DamageCalculator(UPrimitiveComponent* ComponentHit, float BaseDamage);

protected:

	UFUNCTION(BlueprintCallable)
		FVector GetFiringEndLocation(FVector StartPoint, FVector ExactPlayerLocation);
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

private:
	float CurrentHealthPoint;
	int SizeCapsule = 0;
};
