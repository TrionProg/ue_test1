// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"

#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
ATurret::ATurret()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<UStaticMeshComponent>("Base");
	RootComponent = (USceneComponent*)base;

	gun_body = CreateDefaultSubobject<UStaticMeshComponent>("GunBody");
	gun_barrel = CreateDefaultSubobject<UStaticMeshComponent>("GunBarrel");
	gun_lamp = CreateDefaultSubobject<UStaticMeshComponent>("GunLamp");
	gun_muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));

	gun_body->SetupAttachment(RootComponent);
	gun_barrel->SetupAttachment(gun_body);
	gun_lamp->SetupAttachment(gun_body);
	gun_muzzle->SetupAttachment(gun_body);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Base(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'"));
	base->SetStaticMesh(Base.Object);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	gun_body->SetStaticMesh(Cube.Object);
	gun_barrel->SetStaticMesh(Cube.Object); 
	gun_lamp->SetStaticMesh(Cube.Object);

	base->SetWorldScale3D(FVector(1, 1, 1.67));
	gun_body->bAbsoluteRotation = true;
	gun_body->SetWorldLocation(FVector(0.0, 0.0, 150.0));
	gun_body->SetWorldScale3D(FVector(1, 0.2, 0.2));
	gun_barrel->SetRelativeLocation(FVector(TURRET_BARREL_NORMAL_POSITION, 0.0, 30.0));
	gun_barrel->SetRelativeScale3D(FVector(0.6, 0.4, 0.4));
	gun_lamp->SetRelativeLocation(FVector(-15, 0.0, 100.0));
	gun_lamp->SetRelativeScale3D(FVector(0.6, 0.4, 0.2));
	gun_muzzle->SetRelativeLocation(FVector(TURRET_MUZZLE_POSITION, 0.0, 30.0));

	target = OptionPtr<AEnemy>::new_none();
	shot_interval_progress = shot_interval;
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	target.reset();
	shot_interval_progress = shot_interval;
}

// Called every frame
void ATurret::Tick(float dt)
{
	Super::Tick(dt);

	if (shot_interval_progress != shot_interval) {
		shot_interval_progress += dt;

		if (shot_interval_progress > shot_interval) {
			shot_interval_progress = shot_interval;
		}

		animate_shoot();
	}

	if (target.is_none()) {
		find_target();
	}

	if (auto current_target = target.match()) {
		auto location = current_target->GetActorLocation();

		auto rotator = look_at(location, FVector(0, 0, 1));
		gun_body->SetWorldRotation(rotator);

		if (shot_interval_progress == shot_interval) {
			shoot();
		}
	}
}

// Called to bind functionality to input
void ATurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FRotator ATurret::look_at(FVector lookAt, FVector upDirection)
{
	FVector forward = lookAt - GetActorLocation();
	FVector up = upDirection;


	forward = forward.GetSafeNormal();
	up = up - (forward * FVector::DotProduct(up, forward));
	up = up.GetSafeNormal();

	///////////////////////


	FVector vector = forward.GetSafeNormal();
	FVector vector2 = FVector::CrossProduct(up, vector);
	FVector vector3 = FVector::CrossProduct(vector, vector2);
	float m00 = vector.X;
	float m01 = vector.Y;
	float m02 = vector.Z;
	float m10 = vector2.X;
	float m11 = vector2.Y;
	float m12 = vector2.Z;
	float m20 = vector3.X;
	float m21 = vector3.Y;
	float m22 = vector3.Z;

	float num8 = (m00 + m11) + m22;
	FQuat quaternion = FQuat();

	if (num8 > 0.0f)
	{
		float num = (float)FMath::Sqrt(num8 + 1.0f);
		quaternion.W = num * 0.5f;
		num = 0.5f / num;
		quaternion.X = (m12 - m21) * num;
		quaternion.Y = (m20 - m02) * num;
		quaternion.Z = (m01 - m10) * num;
		return FRotator(quaternion);
	}

	if ((m00 >= m11) && (m00 >= m22))
	{
		float num7 = (float)FMath::Sqrt(((1.0f + m00) - m11) - m22);
		float num4 = 0.5f / num7;
		quaternion.X = 0.5f * num7;
		quaternion.Y = (m01 + m10) * num4;
		quaternion.Z = (m02 + m20) * num4;
		quaternion.W = (m12 - m21) * num4;
		return FRotator(quaternion);
	}

	if (m11 > m22)
	{
		float num6 = (float)FMath::Sqrt(((1.0f + m11) - m00) - m22);
		float num3 = 0.5f / num6;
		quaternion.X = (m10 + m01) * num3;
		quaternion.Y = 0.5f * num6;
		quaternion.Z = (m21 + m12) * num3;
		quaternion.W = (m20 - m02) * num3;
		return FRotator(quaternion);
	}

	float num5 = (float)FMath::Sqrt(((1.0f + m22) - m00) - m11);
	float num2 = 0.5f / num5;
	quaternion.X = (m20 + m02) * num2;
	quaternion.Y = (m21 + m12) * num2;
	quaternion.Z = 0.5f * num5;
	quaternion.W = (m01 - m10) * num2;


	return FRotator(quaternion);
}

void ATurret::find_target() {
	TArray<AActor*> found_actors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), found_actors);

	auto position = GetActorLocation();
	auto found_target=OptionPtr<AEnemy>::new_none();
	float min_dist = MAX_FLT;

	for (AActor* abstract_actor : found_actors) {
		AEnemy* enemy = Cast<AEnemy>(abstract_actor);

		if (enemy != nullptr) {
			if (enemy->is_alive()) {
				auto location = enemy->GetActorLocation();

				auto dist = FVector::Dist2D(position, location);

				if (dist < min_dist) {
					//TODO trace (LineTrace)

					min_dist = dist;
					found_target.set(enemy);
				}
			}
		}
	}

	if (auto fnd_target = found_target.match()) {
		target.set(fnd_target);
	}
}

void ATurret::shoot() {
	if (ShootProjectile != NULL)
	{
		UWorld* const world = GetWorld();
		if (world != NULL)
		{
			const auto spawn_rotation = gun_muzzle->GetComponentRotation();
			const auto spawn_location = gun_muzzle->GetComponentLocation();

			UE_LOG(LogTemp, Warning, TEXT("STOT %f %f %f "), spawn_location.X, spawn_location.Y, spawn_location.Z);

			//TODO
			//const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			//const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			world->SpawnActor<AProjectile>(ShootProjectile, spawn_location, spawn_rotation, ActorSpawnParams);

			shot_interval_progress = 0;
		}
	}

	/*
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
	*/
}

void ATurret::animate_shoot() {
	float pos = TURRET_BARREL_NORMAL_POSITION;
	float payback_interval = shot_interval * 0.2;

	if (shot_interval_progress < payback_interval) {// מעהאקא
		auto k = ((payback_interval - shot_interval_progress) / payback_interval);
		pos = TURRET_BARREL_NORMAL_POSITION * k;
	}else if (shot_interval_progress < shot_interval) {
		auto k = ((shot_interval_progress - payback_interval) / (shot_interval * 0.8));
		pos = TURRET_BARREL_NORMAL_POSITION * k;
	}

	gun_barrel->SetRelativeLocation(FVector(pos, 0.0, 30.0));
}

void ATurret::on_enemy_died(AEnemy* enemy) {
	if (auto current_target = target.match()) {
		if (current_target == enemy) {
			UE_LOG(LogTemp, Warning, TEXT("MyEnemy"));
			target.reset();
			find_target();
		}
	}
}

uint8 ATurret::get_type() {
	return type;
}