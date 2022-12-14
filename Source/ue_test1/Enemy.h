// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "primitives.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "MyPawnMovementComponent.h"
#include "OptionPtr.h"

#include "Enemy.generated.h"

UCLASS()
class UE_TEST1_API AEnemy : public ACharacter//APawn
{
	GENERATED_BODY()
//UE variables
protected:
	//UPROPERTY(EditAnyWhere)
	//UShapeComponent* collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* body_base;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* sphere1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* sphere2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* sphere3;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UMyPawnMovementComponent* movement_component;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float MaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float SpeedIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 Reward;
//UE events and methods
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Destroyed(AActor* DestroyedActor);

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void Reset();
public:
	AEnemy();
//My variables
private:
	f32 health;
	f32 speed;
	float body_swing = 0;
//My methods
private:
	OptionPtr<UWorld> get_world();
	OptionPtr<class ABasicGameModeBase> get_game_mode();

	void animate(float dt);
protected:
	UFUNCTION(BlueprintCallable)
	void slow_down(float dmg);
public:	
	int32 get_reward();
	bool is_alive();
	void kill();
};
