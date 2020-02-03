// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "MyHUD.h"


AMyPlayerController::AMyPlayerController() {
	bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI());

	//TODO IsLocalController(). and move to controller
	ChangeMenuWidget(StartingWidgetClass);
}

void AMyPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("move up", this, &AMyPlayerController::move_up);
	InputComponent->BindAxis("move right", this, &AMyPlayerController::move_right);

	InputComponent->BindAction("LMBClick", IE_Pressed, this, &AMyPlayerController::on_lmb_press);
	InputComponent->BindAction("LMBClick", IE_Released, this, &AMyPlayerController::on_lmb_release);
}

void AMyPlayerController::PlayerTick(float dt) {
	Super::PlayerTick(dt);

	draw_money();
}

void AMyPlayerController::move_right(float value) {
	get_spectator()->move_right(value);
}

void AMyPlayerController::move_up(float value) {
	get_spectator()->move_up(value);
}

void AMyPlayerController::on_lmb_press() {
	UE_LOG(LogTemp, Warning, TEXT("Press"));
}

void AMyPlayerController::on_lmb_release() {
	UE_LOG(LogTemp, Warning, TEXT("Click"));

	get_spectator()->build(1);
}

void AMyPlayerController::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (!IsLocalController()) {
		return;
	}

	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

ASpectator* AMyPlayerController::get_spectator() {
	auto pawn = GetPawn();
	auto spectator = (ASpectator*)pawn;

	return spectator;
}

void AMyPlayerController::draw_money() {
	if (CurrentWidget == nullptr) {
		return;
	}

	auto spectator = get_spectator();

	auto money = spectator->get_money();

	auto hud = (UMyHUD*)CurrentWidget;

	hud->SetMoney(money);
}