// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "primitives.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

enum class ETurretType : uint8;

#include "Spectator.generated.h"

UCLASS()
class UE_TEST1_API ASpectator : public APawn
{
	GENERATED_BODY()
//UE variables
private:
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* spring_arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* camera;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementForce;

//UE events and methods
protected:
	UFUNCTION()
	void Destroyed(AActor* DestroyedActor);

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Reset() override;
	virtual void Restart() override;

public:
	// Sets default values for this pawn's properties
	ASpectator();
// My variables
protected:
	float should_move_left;
	float should_move_up;
//My methods and events
private:
	void zoom_in();
	void zoom_out();
public:	
	void move_up(float value);
	void move_right(float value);
};
