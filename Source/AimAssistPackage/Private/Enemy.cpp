// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemySpawner.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HeadZone = CreateDefaultSubobject<UCapsuleComponent>("Head Zone");
	BodyZone = CreateDefaultSubobject<UCapsuleComponent>("Body Zone");
	LegZone = CreateDefaultSubobject<UCapsuleComponent>("Legs Zone");

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), "hand_r");

	ShootingPoint = CreateDefaultSubobject<USceneComponent>("Shooting Point");
	ShootingPoint->SetupAttachment(Weapon);

	BodyZone->SetupAttachment(GetMesh(), "spine_02");
	LegZone->SetupAttachment(GetMesh(), "pelvis");
	HeadZone->SetupAttachment(GetMesh(), "head");
}

void AEnemy::Firing()
{
	if (FireAnimation)
		GetMesh()->GetAnimInstance()->Montage_Play(FireAnimation);

	if (FireSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, ShootingPoint->GetComponentLocation());

	FVector StartLocation = ShootingPoint->GetComponentLocation();
	FVector EndLocation = GetFiringEndLocation(StartLocation, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
	FVector NewDirection = Direction * 1500;
	EndLocation = EndLocation + NewDirection;
	
	FHitResult Hit(ForceInit);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor(255, 0, 0), false, 1, 0, 2);
	FCollisionQueryParams param;
	GetWorld()->LineTraceSingleByProfile(Hit, StartLocation, EndLocation, "Pawn", param);
	if (Hit.GetActor() != nullptr)
	{
		if (Hit.GetActor()->CanBeDamaged())
			Hit.GetActor()->TakeDamage(Damages, FDamageEvent(), GetWorld()->GetFirstPlayerController(), GetWorld()->GetFirstPlayerController()->K2_GetPawn());
	}
}

FVector AEnemy::GetFiringEndLocation(FVector StartPoint, FVector ExactPlayerLocation)
{
	int randInt = FMath::RandRange(0, 100);
	bool randLeftRigh = FMath::RandBool();
	float randHeight = FMath::RandRange(-SizeCapsule, SizeCapsule);


	if (randInt < AimPercentage)
		return ExactPlayerLocation;
	
	FVector Direction = (ExactPlayerLocation - StartPoint).GetSafeNormal();
	float CathetusLength = SizeCapsule + MissingOffset;
	CathetusLength *= (randLeftRigh) ? 1 : -1;
	FVector PerpendicularVector = FVector::CrossProduct(Direction, FVector::UpVector).GetSafeNormal();
	FVector LastPoint = ExactPlayerLocation + PerpendicularVector * CathetusLength;
	LastPoint.Z += randHeight;
	return (LastPoint);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealthPoint = HealthPointMax;
	SizeCapsule = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetCapsuleComponent()->GetScaledCapsuleRadius();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealthPoint -= DamageAmount;
	if (CurrentHealthPoint <= 0)
		K2_DestroyActor();
	return CurrentHealthPoint;
}

void AEnemy::Destroyed()
{
	Super::Destroyed();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), FoundActors);
	Cast<AEnemySpawner>(FoundActors[0])->SpawnActor();
}

float AEnemy::DamageCalculator(UPrimitiveComponent* ComponentHit, float BaseDamage)
{
	if (ComponentHit->GetName() == WeakZoneName)
		return BaseDamage * WeakZoneMultiplier;

	if (ComponentHit->GetName() == MidZoneName)
		return BaseDamage * MidZoneMultiplier;

	if (ComponentHit->GetName() == ResistantZoneName)
		return BaseDamage * ResistantZoneMultiplier;

	return BaseDamage;
}
