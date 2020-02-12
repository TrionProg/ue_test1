// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "MyHUD.h"
#include "BasicGameModeBase.h"
#include "Spectator.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


AMyPlayerController::AMyPlayerController() {
	bShowMouseCursor = true;
	paused = false;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Player Controller begin play"));

	/*

	TArray<AActor*> found_actors;

	UGameplayStatics::GetAllActorsOfClass((UObject*)GetWorld(), ASpectator::StaticClass(), found_actors);

	UE_LOG(LogTemp, Warning, TEXT("Controller beg play"));

	for (AActor* abstract_actor : found_actors) {
		ASpectator* spec = Cast<ASpectator>(abstract_actor);

		if (spec) {
			UE_LOG(LogTemp, Warning, TEXT("Spec!!!!"));
			Possess(spec);
		}
	}
	*/

	SetInputMode(FInputModeGameAndUI());

	//TODO IsLocalController(). and move to controller
	ChangeMenuWidget(StartingWidgetClass);
	select_turret_type(1);
}


void AMyPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("move up", this, &AMyPlayerController::move_up);
	InputComponent->BindAxis("move right", this, &AMyPlayerController::move_right);

	InputComponent->BindAction("LMBClick", IE_Pressed, this, &AMyPlayerController::on_lmb_press);
	InputComponent->BindAction("LMBClick", IE_Released, this, &AMyPlayerController::on_lmb_release);

	InputComponent->BindAction("Turret1", IE_Released, this, &AMyPlayerController::on_key1_release);
	InputComponent->BindAction("Turret2", IE_Released, this, &AMyPlayerController::on_key2_release);
	InputComponent->BindAction("Turret3", IE_Released, this, &AMyPlayerController::on_key3_release);

	InputComponent->BindAction("Pause", IE_Released, this, &AMyPlayerController::on_key_space_release).bExecuteWhenPaused = true;
	InputComponent->BindAction("Restart", IE_Released, this, &AMyPlayerController::on_key_r_release).bExecuteWhenPaused = true;
	//InputComponent->BindAction("TogglePause", EInputEvent::IE_Pressed, this, &MyPlayerController::TogglePause).bExecuteWhenPaused = true;

	//TODO some code:InputComponent->BindAction("TogglePause", EInputEvent::IE_Pressed, this, &MyPlayerController::TogglePause).bExecuteWhenPaused = true;
}

void AMyPlayerController::PlayerTick(float dt) {
	Super::PlayerTick(dt);

	//Костыль
	/*

	if (get_spectator().is_none()) {
		UE_LOG(LogTemp, Warning, TEXT("Kostil"));
		TArray<AActor*> found_actors;

		UGameplayStatics::GetAllActorsOfClass((UObject*)GetWorld(), ASpectator::StaticClass(), found_actors);

		for (AActor* abstract_actor : found_actors) {
			UE_LOG(LogTemp, Warning, TEXT("ggg"));
			ASpectator* spec = Cast<ASpectator>(abstract_actor);

			if (spec) {
				UE_LOG(LogTemp, Warning, TEXT("Spec!!!!"));
				Possess(spec);
			}
		}
	}
	*/

	//UE_LOG(LogTemp, Warning, TEXT("Pl Tick"));

	draw_money();
}

void AMyPlayerController::move_right(float value) {
	if (auto spectator = get_spectator().match()) {
		spectator->move_right(value);
	}
}

void AMyPlayerController::move_up(float value) {
	if (auto spectator = get_spectator().match()) {
		spectator->move_up(value);
	}
}

void AMyPlayerController::on_lmb_press() {
	UE_LOG(LogTemp, Warning, TEXT("Press"));
}

void AMyPlayerController::on_lmb_release() {
	UE_LOG(LogTemp, Warning, TEXT("Click"));

	if (auto spectator = get_spectator().match()) {
		spectator->build();
	}
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

OptionPtr<ASpectator> AMyPlayerController::get_spectator() { //TODO On reset may be nullptr
	auto pawn = GetPawn();
	auto spectator = (ASpectator*)pawn;

	return OptionPtr<ASpectator>::new_unchecked(spectator);
}

void AMyPlayerController::draw_money() {
	if (CurrentWidget == nullptr) {
		return;
	}

	if (auto spectator = get_spectator().match()) {
		auto money = spectator->get_money();

		auto hud = (UMyHUD*)CurrentWidget;

		hud->SetMoney(money);
	}
}

void AMyPlayerController::on_key1_release() {
	select_turret_type(1);
}

void AMyPlayerController::on_key2_release() {
	select_turret_type(2);
}

void AMyPlayerController::on_key3_release() {
	select_turret_type(3);
}

void AMyPlayerController::select_turret_type(uint8 turret_type) {
	if (auto spectator = get_spectator().match()) {
		auto turret_name = spectator->get_turret_name(turret_type);
		auto turret_price = spectator->get_turret_price(turret_type);

		spectator->set_current_turret_type(turret_type);

		if (!IsLocalController()) {
			return;
		}

		auto hud = (UMyHUD*)CurrentWidget;

		hud->SetCurrentTurret(turret_name, turret_price);
	}
}

void AMyPlayerController::on_key_space_release() {
	pause();
}

void AMyPlayerController::on_key_r_release() {
	restart_game();
}

void AMyPlayerController::pause() {
	UE_LOG(LogTemp, Warning, TEXT("Pause"));
	paused = !paused;
	this->SetPause(paused);
}

void AMyPlayerController::restart_game() {
	UE_LOG(LogTemp, Warning, TEXT("Restart"));
	ABasicGameModeBase * game_mode = Cast<ABasicGameModeBase>(GetWorld()->GetAuthGameMode());

	if (game_mode) {
		game_mode->restart_game();
	}
	//game_mode->ResetLevel();
}

/** Reset actor to initial state - used when restarting level without reloading. */
void AMyPlayerController::Reset() {
	Super::Reset();
	UE_LOG(LogTemp, Warning, TEXT("Reset Controller"));

	/*
	//Костыль не работает т.к. далее резет спектатора.

	TArray<AActor*> found_actors;

	UGameplayStatics::GetAllActorsOfClass((UObject*)GetWorld(), ASpectator::StaticClass(), found_actors);

	for (AActor* abstract_actor : found_actors) {
		ASpectator* spec = Cast<ASpectator>(abstract_actor);

		if (spec) {
			//UE_LOG(LogTemp, Warning, TEXT("Spec!!!!"));
			Possess(spec);
			return;
		}
	}
	*/
}