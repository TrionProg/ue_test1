// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingSpot.h"

//TODO may be add FlyingComponent?

//UE events and methods

// Sets default values
AFlyingSpot::AFlyingSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlyingSpot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlyingSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (auto target = hover_on_target.match()) {
		auto target_location = target->GetActorLocation();

		auto location = FVector(
			target_location.X,
			target_location.Y,
			target_location.Z + HoverHeight
		);

		SetActorLocation(location);
	}

}

//My methods

void AFlyingSpot::set_hover_on_target(AActor& target) {
	hover_on_target.set(&target);
}