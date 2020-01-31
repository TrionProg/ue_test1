// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE_TEST1_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
public:
	AMyPlayerController();

protected:
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	
	void move_up(float value);
	void move_right(float value);

	void on_lmb_press();
	void on_lmb_release();
};
