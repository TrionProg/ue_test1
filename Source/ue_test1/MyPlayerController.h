// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Spectator.h"
#include "OptionPtr.h"

#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE_TEST1_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	/** Remove the current menu widget and create a new one from the specified class, if provided. */
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	void draw_money();

	void select_turret_type(uint8 turret_type);

	bool paused;
protected:
	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;
public:
	AMyPlayerController();

	OptionPtr<ASpectator> get_spectator();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	
	void move_up(float value);
	void move_right(float value);

	void on_lmb_press();
	void on_lmb_release();

	void on_key1_release();
	void on_key2_release();
	void on_key3_release();
	void on_key_space_release();
	void on_key_r_release();
	
	void pause();
	void restart_game();

	/** Reset actor to initial state - used when restarting level without reloading. */
	virtual void Reset();
};
