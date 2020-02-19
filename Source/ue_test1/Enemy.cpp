// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

#include "Runtime/Engine/Classes/Components/InputComponent.h"

#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
//#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "MyPlayerController.h"
#include "Turret.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BasicGameModeBase.h"

//UE events and methods

const float BODY_SWING_SPEED = 3.0;
const float BODY_SWING_AMPLITUDE = 8.0;
const float BODY_SWING_SLOW_DOWN_DMG = -0.5;

// Sets default values
AEnemy::AEnemy() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	//RootComponent = collision;

	body_base = CreateDefaultSubobject<USceneComponent>(TEXT("BodyBase"));
	sphere1 = CreateDefaultSubobject<UStaticMeshComponent>("Sphere 1");
	sphere2 = CreateDefaultSubobject<UStaticMeshComponent>("Sphere 2");
	sphere3 = CreateDefaultSubobject<UStaticMeshComponent>("Sphere 3");

	//collision->SetupAttachment(RootComponent);
	body_base->SetupAttachment(RootComponent);
	sphere1->SetupAttachment(body_base);
	sphere2->SetupAttachment(body_base);
	sphere3->SetupAttachment(body_base);

	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	sphere1->SetStaticMesh(MeshToUse.Object);
	sphere2->SetStaticMesh(MeshToUse.Object);
	sphere3->SetStaticMesh(MeshToUse.Object);

	auto collision = GetCapsuleComponent();
	collision->InitCapsuleSize(50.0f, 100.0f);
	//collision->SetGenerateOverlapEvents(true);
	//collision->SetCollisionProfileName(TEXT("Pawn"));

	//collision->SetWorldLocation(FVector(0.0, 0.0, 110.0));
	//collision->SetWorldScale3D(FVector(1,1,3.2));
	//collision->SetGenerateOverlapEvents(true);
	//collision->SetCollisionProfileName(TEXT("Pawn"));
	//collision->bAutoActivate = true; //Activated by default?

	body_base->SetWorldLocation(FVector(0.0, 0.0, -95.0));

	sphere1->SetGenerateOverlapEvents(false);
	sphere1->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	sphere2->SetRelativeLocation(FVector(0.0, 0.0, 90.0));
	sphere2->SetRelativeScale3D(FVector(0.75));
	sphere2->SetGenerateOverlapEvents(false);
	sphere2->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	sphere3->SetRelativeLocation(FVector(0.0, 0.0, 160.0));
	sphere3->SetRelativeScale3D(FVector(0.45));
	sphere3->SetGenerateOverlapEvents(false);
	sphere3->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	/*
	movement_component = CreateDefaultSubobject<UMyPawnMovementComponent>(TEXT("MyMovementComponent"));
	movement_component->UpdatedComponent = RootComponent;
	*/
	//movement_component->bAutoActivate = false; //Activated by default?

	MaxHealth = 100;
	MaxSpeed = 100;
	SpeedIncrease = 10;

	this->OnTakeAnyDamage.AddDynamic(this, &AEnemy::TakeDamage);
	this->OnDestroyed.AddDynamic(this, &AEnemy::Destroyed);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	 
	health = MaxHealth;
	speed = MaxSpeed;
	GetCharacterMovement()->MaxWalkSpeed = speed;
	body_swing = 0.0;
}

// Called every frame
void AEnemy::Tick(float dt)
{
	Super::Tick(dt);

	if (speed < MaxSpeed) {
		speed += SpeedIncrease * dt;

		if (speed > MaxSpeed) {
			speed = MaxSpeed;
		}

		GetCharacterMovement()->MaxWalkSpeed = speed;
	}

	animate(dt);
}

void AEnemy::Destroyed(AActor* DestroyedActor) {
	UE_LOG(LogTemp, Log, TEXT("Destroyed Enemy"));
	//TODO AI Controller will be deleted?

	if (auto world = get_world().match()) {
		TArray<AActor*> found_actors;

		UGameplayStatics::GetAllActorsOfClass((UObject*)world, ATurret::StaticClass(), found_actors);

		for (AActor* abstract_actor : found_actors) {
			UE_LOG(LogTemp, Warning, TEXT("Turret"));
			ATurret* turret = Cast<ATurret>(abstract_actor);

			if (turret) {
				turret->on_enemy_died(this);
			}
		}
	}

	if (auto game_mode = get_game_mode().match()) {
		game_mode->reset_fugitive(*this);
	}
}

void AEnemy::Reset() {
	UE_LOG(LogTemp, Log, TEXT("Reset Enemy"));
	if (auto controller = GetController()) {//без этого он не удалит врага(не вызовет Destroy), видимо из-за сборщика мусора, ведь контроллер есть. 
		controller->UnPossess();//TODO AI Controller will be deleted?
	}
	Super::Reset();
}

//My methods

OptionPtr<UWorld> AEnemy::get_world() {
	return OptionPtr<UWorld>::new_unchecked(GetWorld());
}

OptionPtr<ABasicGameModeBase> AEnemy::get_game_mode() {
	if (auto world = get_world().match()) {
		auto abstract_game_mode = world->GetAuthGameMode();
		if (abstract_game_mode) {
			ABasicGameModeBase* game_mode = Cast<ABasicGameModeBase>(abstract_game_mode);

			return OptionPtr<ABasicGameModeBase>::new_unchecked(game_mode);
		}
	}

	return OptionPtr<ABasicGameModeBase>::new_none();
}

int32 AEnemy::get_reward() {
	return Reward;
}

void AEnemy::slow_down(float dmg) {
	if (speed < dmg) {
		speed = 0;
	}else {
		speed -= dmg;
	}

	GetCharacterMovement()->MaxWalkSpeed = speed;

	body_swing = BODY_SWING_SLOW_DOWN_DMG;
}

void AEnemy::TakeDamage(AActor* DamagedActor, float damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
	if (health <= damage) {
		if (InstigatedBy) {
			auto controller = Cast<AMyPlayerController>(InstigatedBy);

			if (controller) {
				controller->give_money(Reward);
			}
		}

		kill();
	}else {
		health -= damage;
	}
}

bool AEnemy::is_alive() {
	return health > 0;
}

void AEnemy::animate(float dt) {
	body_swing += dt * BODY_SWING_SPEED;

	if (body_swing > 2.0*PI) {
		body_swing -= 2.0*PI;
	}

	float swing = FMath::Sin(body_swing);

	auto new_rotation = FRotator(
		swing * BODY_SWING_AMPLITUDE,
		0.0,
		0.0
	);

	body_base->SetWorldRotation(new_rotation);
}

void AEnemy::kill() {
	health = 0;
	Destroy();
}