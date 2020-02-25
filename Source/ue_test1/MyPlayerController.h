// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OptionPtr.h"

#include "MyPlayerController.generated.h"

class AMyPlayerState;
enum class ETurretType : uint8;

/**
 * 
 */
UCLASS()
class UE_TEST1_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
//UE variables
//UE events and methods
private:
protected:
	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

	virtual void Reset() override;
	virtual void BeginPlayingState() override;
	virtual void EndPlayingState() override;
public:
	AMyPlayerController();
//My variables
private:
	bool paused;
//My methods
private:
	OptionPtr<UWorld> get_world();
	OptionPtr<class ASpectator> get_spectator();
	OptionPtr<class AMyPlayerState> get_player_state();
	OptionPtr<class ABasicGameModeBase> get_game_mode();
private:
	//Removes the current menu widget and create a new one from the specified class, if provided.
	UFUNCTION(BlueprintCallable, Category = "UMG")
	void change_menu_widget(TSubclassOf<UUserWidget> NewWidgetClass);

	void move_up(float value);
	void move_right(float value);

	void on_lmb_release();

	void on_key1_release();
	void on_key2_release();
	void on_key3_release();
	void on_key_space_release();
	void on_key_r_release();

	void on_key_q_press();

	void pause();
	void restart_game();

	void draw_hud();

	void select_turret_type(ETurretType turret_type);

	void build();

public:

	//TODO inherit base class for players and bots
	UFUNCTION(BlueprintCallable, Category = "Money")
	void set_money(float money);

	UFUNCTION(BlueprintCallable, Category = "Money")
	void give_money(float money);

	void set_difficulty_level(FString name);
	void set_difficulty_level_time(int32 time);

	void set_health(int32 health);
	bool decrement_health();

	void on_before_match();
	void on_tick(float dt);
};
