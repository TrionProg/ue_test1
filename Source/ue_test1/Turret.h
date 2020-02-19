// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.h"
#include "Projectile.h"
#include "OptionPtr.h"

#include "Turret.generated.h"

const float TURRET_BARREL_NORMAL_POSITION = 50;
const float TURRET_MUZZLE_POSITION = 90;

UCLASS()
class UE_TEST1_API ATurret : public APawn
{
	GENERATED_BODY()
//UE variables
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* base;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* gun_body;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* gun_barrel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* gun_lamp;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* gun_muzzle;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ShootProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) //EditAnywhere, Category = Projectile
	float shot_interval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) //EditAnywhere
	uint8 type;

//UE events and methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	// Sets default values for this pawn's properties
	ATurret();

//My variables
private:
	OptionPtr<AEnemy> target;
	float shot_interval_progress;

private:
	OptionPtr<UWorld> get_world();

	// »щем близжайшего врага
	void find_target();

	void shoot();

	void animate_shoot();

	//посмотреть куда-то
	FRotator look_at(FVector actor_pos, FVector lookAt, FVector upDirection);
public:	
	void on_enemy_died(AEnemy* enemy);

	uint8 get_type();
};
