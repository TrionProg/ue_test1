// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildSpot.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "Materials/Material.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Turret.h"
#include "TurretType.h"

//UE events and methods
// Sets default values
ABuildSpot::ABuildSpot() : Super()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create a decal in the world to show the cursor's location
	decal = CreateDefaultSubobject<UDecalComponent>("Decal");
	decal->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Materials/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		decal->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	decal->DecalSize = FVector(16.0f, 128.0f, 128.0f);
	decal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	turret = OptionPtr<ATurret>::new_none();
}

void ABuildSpot::BeginPlay()
{
	Super::BeginPlay();
	turret = OptionPtr<ATurret>::new_none();
}

void ABuildSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildSpot::Reset()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset ABuildSpot"));
	Super::Reset();
	turret = OptionPtr<ATurret>::new_none();
}

//My methods

bool ABuildSpot::build(ETurretType turret_type) {
	UE_LOG(LogTemp, Warning, TEXT("Build ABuildSpot"));

	if (auto world = get_world().match()) {
		auto pos = GetActorLocation();

		if (auto existing_turret = turret.match()) {
			//Строим лишь то, что круче
			if ((uint8)turret_type > (uint8)existing_turret->get_type()) {
				// Hides visible components
				existing_turret->SetActorHiddenInGame(true);

				// Disables collision components
				existing_turret->SetActorEnableCollision(false);

				if (auto turret_to_spawn = get_turret(turret_type)) {
					if (spawn_turret(*world, turret_to_spawn, pos)) {
						existing_turret->Destroy();
						return true;
					}
				}
			}
		}else {
			if (auto turret_to_spawn = get_turret(turret_type)) {
				if (spawn_turret(*world, turret_to_spawn, pos)) {
					return true;
				}
			}
		}
	}

	return false;
}

OptionPtr<UWorld> ABuildSpot::get_world() {
	return OptionPtr<UWorld>::new_unchecked(GetWorld());
}

TSubclassOf<class ATurret> ABuildSpot::get_turret(ETurretType turret_type) {
	switch (turret_type) {
	case ETurretType::Machinegun:
		if (!Turret1) return nullptr;

		return Turret1;

		break;
	case ETurretType::Freeze:
		if (!Turret2) return nullptr;

		return Turret2;

		break;
	case ETurretType::Pierce:
		if (!Turret3) return nullptr;

		return Turret3;

		break;
	}

	return nullptr;
}

bool ABuildSpot::spawn_turret(UWorld& world, TSubclassOf<class ATurret>& new_turret, FVector& pos) {
	auto rotation = FRotator(0.0);
	auto spawn_info = FActorSpawnParameters();
	spawn_info.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	auto spawned_turret = world.SpawnActor(new_turret, &pos, &rotation, spawn_info);

	if (spawned_turret) {
		turret.set((ATurret*)spawned_turret);

		return true;
	}

	return false;
}

void ABuildSpot::on_turret_destoyed(class ATurret& reseted_turret) {
	if (auto existing_turret = turret.match()) {
		if (existing_turret == &reseted_turret) {
			turret.reset();
		}
	}
}