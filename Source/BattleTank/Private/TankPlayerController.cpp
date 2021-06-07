// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Tank.h"
#include "Engine/World.h"


void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();

	if (ControlledTank) {
		FString ObjectName = ControlledTank->GetName();
		UE_LOG(LogTemp, Warning, TEXT("TankPlayerController possessing %s"), *ObjectName);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PLAYER CONTROLLER NOT POSSESSING A TANK"));
	}
}


void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimAtCrosshair();
}


ATank* ATankPlayerController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}


void ATankPlayerController::AimAtCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation;

	if (GetSightRayHitLocation(HitLocation))
	{
		GetControlledTank()->AimAt(HitLocation);
	}

}


bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation)
{
	//	Viewport x and y screen-size.
	int32 sx, sy;
	GetViewportSize(sx, sy);

	//	Crosshair location in pixel coordinates.
	FVector2D ScreenLocation = FVector2D(sx * CrosshairX, sy * CrosshairY);

	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) {
		return GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}

	return false;
}


/// <summary>If the crosshair is looking at something within LineTraceRange and outs its location.</summary>
/// <param name="LookDirection">The direction to cast towards.</param>
/// <param name="OutHitLocation">The out location of the crosshair's hit.</param>
bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const
{
	FHitResult OutHit;
	FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = CameraLocation + LookDirection * LineTraceRange;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, CameraLocation, EndLocation, ECC_Visibility)) {
		OutHitLocation = OutHit.Location;
		return true;
	}

	return false;
}


/// <summary>Gets the direction the crosshair is looking at.</summary>
/// <param name="ScreenLocation">The screen location of the crosshair.</param>
/// <param name="OutLookDirection">The out look direction.</param>
bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& OutLookDirection) const
{
	FVector CameraWorldLocation;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, OutLookDirection);
}
