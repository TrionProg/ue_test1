// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Spectator.h"


AMyPlayerController::AMyPlayerController() {
	bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;

}

void AMyPlayerController::PlayerTick(float dt) {
	Super::PlayerTick(dt);

}

void AMyPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("move up", this, &AMyPlayerController::move_up);
	InputComponent->BindAxis("move right", this, &AMyPlayerController::move_right);

	InputComponent->BindAction("LMBClick", IE_Pressed, this, &AMyPlayerController::on_lmb_press);
	InputComponent->BindAction("LMBClick", IE_Released, this, &AMyPlayerController::on_lmb_release);
}

void AMyPlayerController::move_right(float value) {
	auto pawn = GetPawn();
	auto spectator = (ASpectator*)pawn;

	spectator->move_right(value);
}

void AMyPlayerController::move_up(float value) {
	auto pawn = GetPawn();
	auto spectator = (ASpectator*)pawn;

	spectator->move_up(value);
}

void AMyPlayerController::on_lmb_press() {
	UE_LOG(LogTemp, Warning, TEXT("Press"));
}

void AMyPlayerController::on_lmb_release() {
	UE_LOG(LogTemp, Warning, TEXT("Click"));

	auto pawn = GetPawn();
	auto spectator = (ASpectator*)pawn;

	spectator->build(1);
}