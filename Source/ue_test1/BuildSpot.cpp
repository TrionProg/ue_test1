// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildSpot.h"


// Sets default values
ABuildSpot::ABuildSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildSpot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

