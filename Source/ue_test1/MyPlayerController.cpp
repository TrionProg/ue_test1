// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "MyHUD.h"
#include "Spectator.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Spectator.h"
#include "MyPlayerState.h"
#include "BasicGameModeBase.h"
#include "BasicGameStateBase.h"
#include "TurretType.h"

//UE events and methods

AMyPlayerController::AMyPlayerController() : Super() {
	bShowMouseCursor = true;
	paused = false;
	CurrentWidget = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("Player Controller Constructor"));
	//DefaultMouseCursor = EMouseCursor::Crosshairs;

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Player Controller begin play"));

	SetInputMode(FInputModeGameAndUI());


}


void AMyPlayerController::SetupInputComponent() {
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("move up", this, &AMyPlayerController::move_up).bExecuteWhenPaused = true;
	InputComponent->BindAxis("move right", this, &AMyPlayerController::move_right).bExecuteWhenPaused = true;

	InputComponent->BindAction("LMBClick", IE_Released, this, &AMyPlayerController::on_lmb_release);

	InputComponent->BindAction("Turret1", IE_Released, this, &AMyPlayerController::on_key1_release);
	InputComponent->BindAction("Turret2", IE_Released, this, &AMyPlayerController::on_key2_release);
	InputComponent->BindAction("Turret3", IE_Released, this, &AMyPlayerController::on_key3_release);

	InputComponent->BindAction("Pause", IE_Released, this, &AMyPlayerController::on_key_space_release).bExecuteWhenPaused = true;
	InputComponent->BindAction("Restart", IE_Released, this, &AMyPlayerController::on_key_r_release).bExecuteWhenPaused = true;
	InputComponent->BindAction("Quit", IE_Pressed, this, &AMyPlayerController::on_key_q_press).bExecuteWhenPaused = true;
	//InputComponent->BindAction("TogglePause", EInputEvent::IE_Pressed, this, &MyPlayerController::TogglePause).bExecuteWhenPaused = true;

	//TODO some code:InputComponent->BindAction("TogglePause", EInputEvent::IE_Pressed, this, &MyPlayerController::TogglePause).bExecuteWhenPaused = true;
}

void AMyPlayerController::PlayerTick(float dt) {
	Super::PlayerTick(dt);

	on_tick(dt);

	draw_hud();
}

void AMyPlayerController::Reset() {
	Super::Reset();
	UE_LOG(LogTemp, Warning, TEXT("Reset Controller"));
}

void AMyPlayerController::BeginPlayingState() {
	UE_LOG(LogTemp, Warning, TEXT("BeginPlayingState"));
	Super::BeginPlayingState();

	if (!IsLocalController()) {
		UE_LOG(LogTemp, Warning, TEXT("NotLocal"));
	}else {
		UE_LOG(LogTemp, Warning, TEXT("IsLocal"));
	}
}

void AMyPlayerController::EndPlayingState() {
	UE_LOG(LogTemp, Warning, TEXT("EndPlayingState"));
	Super::EndPlayingState();
}

//My methods

OptionPtr<UWorld> AMyPlayerController::get_world() {
	return OptionPtr<UWorld>::new_unchecked(GetWorld());
}

OptionPtr<ASpectator> AMyPlayerController::get_spectator() {
	auto pawn = GetPawn();
	auto spectator = (ASpectator*)pawn;

	return OptionPtr<ASpectator>::new_unchecked(spectator);
}

OptionPtr<AMyPlayerState> AMyPlayerController::get_player_state() {
	return OptionPtr<AMyPlayerState>::new_unchecked((AMyPlayerState*)PlayerState);
}

OptionPtr<ABasicGameModeBase> AMyPlayerController::get_game_mode() {
	if (auto world = get_world().match()) {
		auto abstract_game_mode = world->GetAuthGameMode();
		if (abstract_game_mode) {
			ABasicGameModeBase* game_mode = Cast<ABasicGameModeBase>(abstract_game_mode);

			return OptionPtr<ABasicGameModeBase>::new_unchecked(game_mode);
		}
	}

	return OptionPtr<ABasicGameModeBase>::new_none();
}

OptionPtr<ABasicGameStateBase> AMyPlayerController::get_game_state() {
	if (auto world = get_world().match()) {
		return OptionPtr<ABasicGameStateBase>::new_unchecked(world->GetGameState<ABasicGameStateBase>());
	}else {
		return OptionPtr<ABasicGameStateBase>::new_none();
	}
}

void AMyPlayerController::change_menu_widget(TSubclassOf<UUserWidget> NewWidgetClass)
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

void AMyPlayerController::on_lmb_release() {
	build();
}

void AMyPlayerController::on_key1_release() {
	select_turret_type(ETurretType::Machinegun);
}

void AMyPlayerController::on_key2_release() {
	select_turret_type(ETurretType::Freeze);
}

void AMyPlayerController::on_key3_release() {
	select_turret_type(ETurretType::Pierce);
}

void AMyPlayerController::on_key_space_release() {
	pause();
}

void AMyPlayerController::on_key_r_release() {
	restart_game();
}

void AMyPlayerController::on_key_q_press() {
	if (auto world = get_world().match()) {
		UKismetSystemLibrary::QuitGame(world, this, EQuitPreference::Type::Quit);
	}
}

void AMyPlayerController::pause() {
	UE_LOG(LogTemp, Warning, TEXT("Pause"));
	paused = !paused;
	this->SetPause(paused);
}

void AMyPlayerController::restart_game() {
	ABasicGameModeBase * game_mode = Cast<ABasicGameModeBase>(GetWorld()->GetAuthGameMode());

	if (game_mode) {
		game_mode->restart_game();
	}
}

void AMyPlayerController::draw_hud() {
	if (!IsLocalController()) {
		return;
	}

	if (CurrentWidget == nullptr) {
		return;
	}

	auto hud = (UMyHUD*)CurrentWidget;

	if (auto player_state = get_player_state().match()) {
		hud->SetMoney(player_state->money);
	}

	if (auto game_state = get_game_state().match()) {
		hud->SetDifficultyLevelTime(game_state->get_level_time());
	}
}

void AMyPlayerController::select_turret_type(ETurretType turret_type) {
	if (auto player_state = get_player_state().match()) {
		if (auto game_mode = get_game_mode().match()) {
			player_state->current_turret_type = turret_type;
			auto turret_name = game_mode->get_turret_name(turret_type);
			auto turret_price = game_mode->get_turret_price(turret_type);

			if (IsLocalController() && CurrentWidget) {
				auto hud = (UMyHUD*)CurrentWidget;

				hud->SetCurrentTurret(turret_name, turret_price);
			}
		}
	}
}

void AMyPlayerController::build() {
	if (auto spectator = get_spectator().match()) {
		if (auto game_mode = get_game_mode().match()) {
			if (auto player_state = get_player_state().match()) {
				auto turret_type = player_state->current_turret_type;
				auto turret_name = game_mode->get_turret_name(turret_type);
				auto turret_price = game_mode->get_turret_price(turret_type);

				if (player_state->money < turret_price) {
					return;
				}

				if (spectator->build(turret_type)) {
					player_state->money -= turret_price;
				}
			}
		}

	}
}

void AMyPlayerController::set_money(float money) {
	if (auto player_state = get_player_state().match()) {
		player_state->money = money;
	}
}

void AMyPlayerController::give_money(float money) {
	if (auto player_state = get_player_state().match()) {
		player_state->money += money;
	}
}

void AMyPlayerController::set_difficulty_level(FString name) {
	if (IsLocalController() && CurrentWidget) {
		auto hud = (UMyHUD*)CurrentWidget;

		hud->SetCurrentDifficultyLevel(name);
	}
}

void AMyPlayerController::set_health(int32 health) {
	if (auto player_state = get_player_state().match()) {
		player_state->health = health;

		if (IsLocalController() && CurrentWidget) {//TODO CurrentWidget may be nullptr!
			auto hud = (UMyHUD*)CurrentWidget;

			hud->SetPlayerHealth(health);
		}
	}
}

bool AMyPlayerController::decrement_health() {
	if (auto player_state = get_player_state().match()) {
		player_state->health--;

		if (IsLocalController()) {
			auto hud = (UMyHUD*)CurrentWidget;

			hud->SetPlayerHealth(player_state->health);
		}

		return player_state->health == 0;
	}

	return false;
}

void AMyPlayerController::on_before_match() {
	if (!CurrentWidget) {
		change_menu_widget(StartingWidgetClass);
		select_turret_type(ETurretType::Machinegun);
	}
}

void AMyPlayerController::on_tick(float dt) {
	if (auto player_state = get_player_state().match()) {
		
	}
}