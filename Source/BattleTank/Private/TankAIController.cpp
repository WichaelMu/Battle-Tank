// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"


void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	ATank* ControlledTank = GetControlledTank();
	if (ControlledTank) {
		FString ObjectName = ControlledTank->GetName();
		ATank* PlayerTank = GetPlayerTank();

		if (PlayerTank) {
			FString PlayerName = PlayerTank->GetName();
			UE_LOG(LogTemp, Warning, TEXT("TankAIController possessing %s and found %s"), *ObjectName, *PlayerName);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("TankAIController possessing %s but could not find Player"), *ObjectName);
		}

	}

}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ATank* PlayerTank = GetPlayerTank();
	if (PlayerTank) {
		GetControlledTank()->AimAt(PlayerTank->GetActorLocation());
	}
}


ATank* ATankAIController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}


ATank* ATankAIController::GetPlayerTank() const
{
	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (PlayerTank) {
		return Cast<ATank>(PlayerTank);
	}

	UE_LOG(LogTemp, Error, TEXT("CAN'T FIND PLAYER TANK"));

	return nullptr;
}