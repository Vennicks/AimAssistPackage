// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HeadZone = CreateDefaultSubobject<UCapsuleComponent>("Head Zone");
	BodyZone = CreateDefaultSubobject<UCapsuleComponent>("Body Zone");
	LegZone = CreateDefaultSubobject<UCapsuleComponent>("Legs Zone");
	Hitboxes = CreateDefaultSubobject<USceneComponent>("Hitboxes holder");

	Hitboxes->SetupAttachment(RootComponent);
	BodyZone->SetupAttachment(Hitboxes);
	LegZone->SetupAttachment(Hitboxes);
	HeadZone->SetupAttachment(Hitboxes);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}