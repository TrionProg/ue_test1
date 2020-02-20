// Fill out your copyright notice in the Description page of Project Settings.

#include "TurretDestroyer.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Enemy.h"
#include "Turret.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BasicGameModeBase.h"
#include "MyPawnMovementComponent.h"


// Sets default values
ATurretDestroyer::ATurretDestroyer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collision = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = collision;

	sphere = CreateDefaultSubobject<UStaticMeshComponent>("Sphere");
	sphere->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	sphere->SetStaticMesh(MeshToUse.Object);

	sphere->SetRelativeLocation(FVector(0.0, 0.0, -100.0));
	sphere->SetRelativeScale3D(FVector(2.0));
	sphere->SetGenerateOverlapEvents(false);
	sphere->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	collision->InitSphereRadius(100.0f);
	collision->BodyInstance.SetCollisionProfileName("Pawn");

	movement_component = CreateDefaultSubobject<UMyPawnMovementComponent>(TEXT("MyMovementComponent"));
	movement_component->SetUpdatedComponent(RootComponent);//TODO check types of assignment
	//movement_component->UpdatedComponent = RootComponent;

	Speed = 200;

	collision->OnComponentHit.AddDynamic(this, &ATurretDestroyer::OnHit);
}

// Called when the game starts or when spawned
void ATurretDestroyer::BeginPlay()
{
	Super::BeginPlay();

	movement_component->Speed = Speed;
	
}

// Called every frame
void ATurretDestroyer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (auto game_mode = get_game_mode().match()) {
		auto location = GetActorLocation();

		if (location.X < game_mode->LosePosition) {
			Destroy();

			//TODO may be damage player?

			return;
		}
	}

	movement_component->AddInputVector(FVector(-1, 0, 0));
}

/*
// Called to bind functionality to input
void ATurretDestroyer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
*/

OptionPtr<UWorld> ATurretDestroyer::get_world() {
	return OptionPtr<UWorld>::new_unchecked(GetWorld());
}

OptionPtr<ABasicGameModeBase> ATurretDestroyer::get_game_mode() {
	if (auto world = get_world().match()) {
		auto abstract_game_mode = world->GetAuthGameMode();
		if (abstract_game_mode) {
			ABasicGameModeBase* game_mode = Cast<ABasicGameModeBase>(abstract_game_mode);

			return OptionPtr<ABasicGameModeBase>::new_unchecked(game_mode);
		}
	}

	return OptionPtr<ABasicGameModeBase>::new_none();
}

void ATurretDestroyer::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor->IsA(AEnemy::StaticClass())) {
		AEnemy* enemy = Cast<AEnemy>(OtherActor);
		enemy->kill();
	}

	if (OtherActor->IsA(ATurret::StaticClass())) {
		ATurret* turret = Cast<ATurret>(OtherActor);
		turret->Destroy();
		Destroy();
	}
}

