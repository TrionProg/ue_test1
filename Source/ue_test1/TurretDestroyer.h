// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OptionPtr.h"
#include "TurretDestroyer.generated.h"

UCLASS()
class UE_TEST1_API ATurretDestroyer : public APawn
{
	GENERATED_BODY()
//UE variables
protected:
	UPROPERTY(EditAnyWhere)
	class USphereComponent* collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMyPawnMovementComponent* movement_component;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretDestroyer")
	float Speed;

//UE events and methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	// Sets default values for this pawn's properties
	ATurretDestroyer();

//My variables

//My methods
private:
	OptionPtr<UWorld> get_world();
	OptionPtr<class ABasicGameModeBase> get_game_mode();
protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
