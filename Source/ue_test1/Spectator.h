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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	FString turret1_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	FString turret2_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	FString turret3_name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	int32 turret1_price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	int32 turret2_price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Turret)
	int32 turret3_price;

private:
	float money;

	uint8 current_turret_type;

protected:
	UFUNCTION()
	void Destroyed(AActor* DestroyedActor);

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

	bool build();
	
	int32 get_money();
	uint8 get_current_turret_type();
	void set_current_turret_type(uint8 turret_type);
	FString get_turret_name(uint8 turret_type);
	int32 get_turret_price(uint8 turret_type);
	void give_money(int32 add_money);
	void Reset();

	/** Called when the Pawn is being restarted (usually by being possessed by a Controller). */
	virtual void Restart();
};
