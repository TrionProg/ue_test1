// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

#include "Runtime/Engine/Classes/Components/InputComponent.h"

#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	sphere1 = CreateDefaultSubobject<UStaticMeshComponent>("Sphere 1");
	sphere2 = CreateDefaultSubobject<UStaticMeshComponent>("Sphere 2");
	sphere3 = CreateDefaultSubobject<UStaticMeshComponent>("Sphere 3");

	/*
	//WORKS
	FName name2 = *FString::Printf(TEXT("Cude %i"), 2);
	abc = CreateDefaultSubobject<UStaticMeshComponent>(name2);
	*/

	//Do not works?
	//abc = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere 2"));

	//WORKS
	//abc = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cude"));

	/*
	//WORKS
	int32 amount = 3;

	//RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");

	for (int32 i = 0; i < amount; i++) {

		FName name = *FString::Printf(TEXT("Sphere %i"), i);
		UStaticMeshComponent* item = CreateDefaultSubobject<UStaticMeshComponent>(name);
		item->AttachTo(this->RootComponent);

		//abc = item;
	}
	*/

	//sphere3 = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	
	sphere1->SetupAttachment(RootComponent);
	sphere2->SetupAttachment(RootComponent);
	sphere3->SetupAttachment(RootComponent);

	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	sphere1->SetStaticMesh(MeshToUse.Object);
	sphere2->SetStaticMesh(MeshToUse.Object);
	sphere3->SetStaticMesh(MeshToUse.Object);

	
	sphere1->SetWorldLocation(FVector(0.0, 0.0, 0.0));
	sphere2->SetWorldLocation(FVector(0.0, 0.0, 90.0));
	sphere2->SetWorldScale3D(FVector(0.75));
	sphere3->SetRelativeLocation(FVector(0.0, 0.0, 160.0));
	sphere3->SetRelativeScale3D(FVector(0.45));

	max_health = 100;
	max_speed = 1;
	speed_increase = 0.1;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	health = max_health;
	speed = max_speed;
}

// Called every frame
void AEnemy::Tick(float dt)
{
	Super::Tick(dt);

	if (speed < max_speed) {
		speed += speed_increase * dt;

		if (speed > max_speed) {
			speed = max_speed;
		}
	}
}

