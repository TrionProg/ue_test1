// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

#include "Runtime/Engine/Classes/Components/InputComponent.h"

#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "MyPlayerController.h"
#include "Turret.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));

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

	collision->SetupAttachment(RootComponent);
	sphere1->SetupAttachment(RootComponent);
	sphere2->SetupAttachment(RootComponent);
	sphere3->SetupAttachment(RootComponent);

	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	sphere1->SetStaticMesh(MeshToUse.Object);
	sphere2->SetStaticMesh(MeshToUse.Object);
	sphere3->SetStaticMesh(MeshToUse.Object);

	collision->SetWorldLocation(FVector(0.0, 0.0, 110.0));
	collision->SetWorldScale3D(FVector(1,1,3.2));
	collision->SetGenerateOverlapEvents(true);
	collision->SetCollisionProfileName(TEXT("BlockAll"));

	sphere1->SetWorldLocation(FVector(0.0, 0.0, 0.0));
	sphere1->SetGenerateOverlapEvents(false);

	sphere2->SetWorldLocation(FVector(0.0, 0.0, 90.0));
	sphere2->SetWorldScale3D(FVector(0.75));
	sphere2->SetGenerateOverlapEvents(false);

	sphere3->SetRelativeLocation(FVector(0.0, 0.0, 160.0));
	sphere3->SetRelativeScale3D(FVector(0.45));
	sphere3->SetGenerateOverlapEvents(false);

	max_health = 100;
	max_speed = 1;
	speed_increase = 0.1;

	this->OnTakeAnyDamage.AddDynamic(this, &AEnemy::TakeDamage);
	this->OnDestroyed.AddDynamic(this, &AEnemy::Destroyed);
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

int32 AEnemy::get_reward() {
	return reward;
}

void AEnemy::SlowDown(float dmg) {
	UE_LOG(LogTemp, Warning, TEXT("Slow"));
	if (speed < dmg) {
		speed = 0;
	}
	else {
		speed -= dmg;
	}
}

void AEnemy::TakeDamage(AActor* DamagedActor, float damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
	if (health <= damage) {
		health = 0;

		if (InstigatedBy) {
			auto controller = Cast<AMyPlayerController>(InstigatedBy);

			if (controller) {
				auto spectator = controller->get_spectator();

				if (spectator) {
					auto reward = get_reward();
					spectator->give_money(reward);
				}
			}
		}

		Destroy();
	}else {
		health -= damage;
	}
}

void AEnemy::Destroyed(AActor* DestroyedActor) {
	UE_LOG(LogTemp, Warning, TEXT("Destroyed"));

	auto world = GetWorld();

	if (world) {
		TArray<AActor*> found_actors;

		UGameplayStatics::GetAllActorsOfClass((UObject*)world, ATurret::StaticClass(), found_actors);

		for (AActor* abstract_actor : found_actors) {
			UE_LOG(LogTemp, Warning, TEXT("Turret"));
			ATurret* turret = Cast<ATurret>(abstract_actor);

			turret->on_enemy_died(this);
		}
	}
}

bool AEnemy::is_alive() {
	return health > 0;
}