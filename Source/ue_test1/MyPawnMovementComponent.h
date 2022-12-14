// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MyPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UE_TEST1_API UMyPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
//UE variables	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
};