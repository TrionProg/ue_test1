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
	if (auto existing_turret = turret.match()) {
		//������ ���� ��, ��� �����
		if ((uint8)turret_type > (uint8)existing_turret->get_type()) {
			existing_turret->Destroy();
			turret.reset();
		}else {
			return false;
		}
	}

	//TODO ������� ��������� �������, � ���� ����� �������
	//� ��� ������, ��� ������������ ������ ����� ������. ���� ������ ���������, ���� �� ���-�� � �������
	//TODO spawn properties

	if (auto world = get_world().match()) {
		auto pos = GetActorLocation();

		switch (turret_type) {
		case ETurretType::Machinegun:
			if (!Turret1) return false;//TODO assert?

			return spawn_turret(*world, Turret1, pos);

			break;
		case ETurretType::Freeze:
			if (!Turret2) return false;//TODO assert?

			return spawn_turret(*world, Turret2, pos);

			break;
		case ETurretType::Pierce:
			if (!Turret3) return false;//TODO assert?

			return spawn_turret(*world, Turret3, pos);

			break;
		}
	}

	return false;
}

OptionPtr<UWorld> ABuildSpot::get_world() {
	return OptionPtr<UWorld>::new_unchecked(GetWorld());
}

bool ABuildSpot::spawn_turret(UWorld& world, TSubclassOf<class ATurret>& new_turret, FVector& pos) {
	auto spawned_turret = world.SpawnActor(new_turret, &pos);

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