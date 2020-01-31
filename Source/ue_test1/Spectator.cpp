// Fill out your copyright notice in the Description page of Project Settings.

#include "Spectator.h"

#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "BuildSpot.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BasicGameModeBase.h"


// Sets default values
ASpectator::ASpectator()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	spring_arm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	//RootComponent = Mesh;
	spring_arm->SetupAttachment(RootComponent);
	camera->SetupAttachment(spring_arm, USpringArmComponent::SocketName); //TODO , USpringArmComponent::SocketName ??

	RootComponent->SetWorldLocation(FVector(0.0, 0.0, 200.0));
	spring_arm->bAbsoluteRotation = true;

	//spring_arm->SetWorldRotation(FRotator(-60.0, -90.0, 0.0)); //Pitch, Yaw, Roll
	auto quat = FRotator(-60.0, -90.0, 0.0).Quaternion(); //Convert to quaternion
	spring_arm->SetWorldRotation(quat);

	spring_arm->TargetArmLength = 1200.0;

	movement_force = 8.0;

	//Take control of the default Player
	//AutoPossessPlayer = EAutoReceiveInput::Player0; //TODO ??

	//TODO установить модели из ++

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(TEXT("StaticMesh'/Game/MyMesh.MyMesh");
	//MeshComponent->SetStaticMesh(MeshToUse.Object);
}

// Called when the game starts or when spawned
void ASpectator::BeginPlay()
{
	Super::BeginPlay();

	ABasicGameModeBase * game_mode = Cast<ABasicGameModeBase>(GetWorld()->GetAuthGameMode());
	money = (float)game_mode->get_start_money();
	
}

// Called every frame
void ASpectator::Tick(float dt)
{
	Super::Tick(dt);

	/*
	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
	*/

}

// Called to bind functionality to input
void ASpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent = PlayerInputComponent;

	//InputComponent->BindAxis("move up", this, &ASpectator::move_up);
	//InputComponent->BindAxis("move right", this, &ASpectator::move_right);
}

void ASpectator::move_right(float value) {
	FVector force_to_add = movement_force * value * FVector(0, 1, 0);
	auto root = (USceneComponent*)RootComponent;
	root->AddLocalOffset(force_to_add);
}

void ASpectator::move_up(float value) {
	FVector force_to_add = movement_force * value * FVector(1, 0, 0);
	auto root = (USceneComponent*)RootComponent;
	root->AddLocalOffset(force_to_add);
}

bool ASpectator::build(uint8 turret_type) {
	if (APlayerController* PC = Cast<APlayerController>(GetController())) {
		FHitResult TraceHitResult;
		PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
		FVector cursor_position = TraceHitResult.Location;

		TArray<AActor*> found_actors;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildSpot::StaticClass(), found_actors);

		for (AActor* abstract_actor : found_actors) {
			ABuildSpot* build_spot = Cast<ABuildSpot>(abstract_actor);

			if (build_spot != nullptr) {
				auto location = build_spot->GetActorLocation();
				auto dist = FVector::Dist2D(cursor_position, location);

				if (dist <BUILDSPOT_CLICK_RADUS) {
					return build_spot->build(turret_type);
				}
			}
		}
	}

	return false;
}

int32 ASpectator::get_money() {
	return (int32)money;
}