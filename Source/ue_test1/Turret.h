// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemy.h"
#include "Projectile.h"

#include "Turret.generated.h"

UCLASS()
class UE_TEST1_API ATurret : public APawn
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* base;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* gun_body;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* gun_barrel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* gun_lamp;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ShootProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) //EditAnywhere, Category = Projectile
	float shot_interval;

private:
	AEnemy* target;
	float shot_interval_progress;
	//float 
public:
	// Sets default values for this pawn's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	// »щем близжайшего врага
	void find_target();

	void shoot();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//посмотреть куда-то
	FRotator look_at(FVector lookAt, FVector upDirection);

	void on_enemy_died(AEnemy* enemy);
	
};
