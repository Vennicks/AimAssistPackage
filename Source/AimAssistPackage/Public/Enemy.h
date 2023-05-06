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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Zone")
		UCapsuleComponent* HeadZone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Zone")
		UCapsuleComponent* BodyZone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body Zone")
		UCapsuleComponent* LegZone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitboxes")
		USceneComponent* Hitboxes;

	AEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
