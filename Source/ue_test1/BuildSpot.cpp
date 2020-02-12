// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildSpot.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "Materials/Material.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
ABuildSpot::ABuildSpot()
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

// Called when the game starts or when spawned
void ABuildSpot::BeginPlay()
{
	Super::BeginPlay();
	turret = OptionPtr<ATurret>::new_none();
}

// Called every frame
void ABuildSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildSpot::Reset()
{
	turret = OptionPtr<ATurret>::new_none();
}

bool ABuildSpot::build(uint8 turret_type) {
	if (auto existing_turret = turret.match()) {
		//Строим лишь то, что круче
		if (turret_type > existing_turret->get_type()) {
			existing_turret->Destroy();
			turret.reset();
		}else {
			return false;
		}
	}

	if (auto world = GetWorld()) {
		auto pos = GetActorLocation();

		switch (turret_type) {
		case 1:
			if (!Turret1) return false;//TODO assert?

			turret.set((ATurret*)world->SpawnActor(Turret1, &pos));
			break;
		case 2:
			if (!Turret2) return false;//TODO assert?

			turret.set((ATurret*)world->SpawnActor(Turret2, &pos));
			break;
		case 3:
			if (!Turret3) return false;//TODO assert?

			turret.set((ATurret*)world->SpawnActor(Turret3, &pos));
			break;
		}

		return true;
	}else {
		return false;
	}
}

