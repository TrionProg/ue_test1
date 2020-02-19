// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OptionPtr.h"

#include "FlyingSpot.generated.h"

UCLASS()
class UE_TEST1_API AFlyingSpot : public AActor
{
	GENERATED_BODY()

//UE variables
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoverHeight;
//UE events and methods
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:	
	// Sets default values for this actor's properties
	AFlyingSpot();

//My variables
private:
	OptionPtr<class AActor> hover_on_target;

//My methods
public:
	void set_hover_on_target(AActor& target);
	
};
