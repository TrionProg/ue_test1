// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "primitives.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class UE_TEST1_API AEnemy : public AActor
{
	GENERATED_BODY()
private:
	f32 health;
	f32 speed;

protected:
	UPROPERTY(EditAnyWhere)
	UShapeComponent* collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* sphere1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* sphere2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* sphere3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float max_speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float max_health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed_increase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 reward;
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SlowDown(float dmg);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 get_reward();
	
};
