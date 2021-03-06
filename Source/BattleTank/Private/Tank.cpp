// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Engine/World.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "TankMovementComponent.h"
#include "TankAimingComponent.h"

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//	Create Tank Aiming Component and Tank Movement Component on default startup.
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Tank Aiming Component"));
}


// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ATank::AimAt(FVector HitLocation)
{
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
	TankAimingComponent->SetBarrelReference(BarrelToSet);
}

void ATank::SetTurretReference(UTankTurret* TurretToSet)
{
	TankAimingComponent->SetTurretReference(TurretToSet);
}

void ATank::Fire()
{
	double Seconds = FPlatformTime::Seconds();
	bool bIsReloaded = Seconds - LastFireTime > ReloadTimeInSeconds;

	if (Barrel && bIsReloaded)
	{
		FVector SpawnLocation = Barrel->GetSocketLocation(FName("Projectile"));
		FRotator SpawnRotation = Barrel->GetSocketRotation(FName("Projectile"));

		AProjectile* _Projectile = GetWorld()->SpawnActor<AProjectile>(Projectile, SpawnLocation, SpawnRotation);

		_Projectile->LaunchProjectile(LaunchSpeed);

		LastFireTime = Seconds;
	}
}

