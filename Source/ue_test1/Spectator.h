// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "primitives.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Spectator.generated.h"

UCLASS()
class UE_TEST1_API ASpectator : public APawn
{
	GENERATED_BODY()

private:
	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* spring_arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float movement_force;

private:
	float money;

public:
	// Sets default values for this pawn's properties
	ASpectator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void move_up(float value);
	void move_right(float value);

	bool build(uint8 turret_type);
	
	int32 get_money();
};
