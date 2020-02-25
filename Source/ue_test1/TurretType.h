// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETurretType : uint8
{
	Machinegun 	UMETA(DisplayName = "Machinegun -- just shoot in enemies"),
	Freeze 		UMETA(DisplayName = "Freeze -- damage and freeze enemies"),
	Pierce		UMETA(DisplayName = "Pierce multiple enemies")
};