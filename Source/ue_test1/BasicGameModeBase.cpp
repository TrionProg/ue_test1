// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicGameModeBase.h"
#include "Spectator.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


ABasicGameModeBase::ABasicGameModeBase(): Super() {
	PrimaryActorTick.bCanEverTick = true;

	//Use C++ Spectator
	DefaultPawnClass = ASpectator::StaticClass(); 

	/*
	//Use blueprint Specrator
	static ConstructorHelpers::FClassFinder<APawn> FoundFPC(TEXT("/Game/Blueprints/SpectatorBlueprint"));
	if (FoundFPC.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found FPC"));

		DefaultPawnClass = (UClass*)FoundFPC.Class;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FPC Not found"));
		DefaultPawnClass = nullptr;
	}
	*/
}


void ABasicGameModeBase::Tick(float DeltaSeconds) {
	UE_LOG(LogTemp, Warning, TEXT("Tick"));
}