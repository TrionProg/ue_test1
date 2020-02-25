// Fill out your copyright notice in the Description page of Project Settings.

#include "Spectator.h"

#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "BuildSpot.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Turret.h"
#include "BasicGameModeBase.h"
//#include "Turret.h"

const float ZOOM_STEP = 100;
const float MIN_ZOOM_DIST = 400;
const float MAX_ZOOM_DIST = 2000;

//UE events and methods

// Sets default values
ASpectator::ASpectator() : Super()
{
	//Super::AActor();
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

	MovementForce = 100;

	should_move_left = 0;
	should_move_up = 0;

	//Take control of the default Player
	//AutoPossessPlayer = EAutoReceiveInput::Player0; //TODO ??

	//TODO установить модели из ++

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshToUse(TEXT("StaticMesh'/Game/MyMesh.MyMesh");
	//MeshComponent->SetStaticMesh(MeshToUse.Object);
	this->OnDestroyed.AddDynamic(this, &ASpectator::Destroyed);
}

// Called when the game starts or when spawned
void ASpectator::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Spec begin play"));
	Super::BeginPlay();

	should_move_left = 0;
	should_move_up = 0;
}

// Called to bind functionality to input
void ASpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent = PlayerInputComponent;

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ASpectator::zoom_in);//TODO may be and released
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ASpectator::zoom_out);

	//InputComponent->BindAxis("move up", this, &ASpectator::move_up);
	//InputComponent->BindAxis("move right", this, &ASpectator::move_right);
}

// Called every frame
void ASpectator::Tick(float dt)
{
	Super::Tick(dt);

	auto location = GetActorLocation();
	auto new_location = FVector(
		location.X + should_move_left * MovementForce * dt,
		location.Y + should_move_up * MovementForce * dt,
		location.Z
	);

	SetActorLocation(new_location);
}

void ASpectator::Reset() {
	Super::Reset();
	UE_LOG(LogTemp, Warning, TEXT("Reset Spec"));

	//TODO maybe possess here?
}

void ASpectator::Restart() {
	Super::Restart();
	UE_LOG(LogTemp, Warning, TEXT("Restart Spec"));
}

void ASpectator::Destroyed(AActor* DestroyedActor) {
	UE_LOG(LogTemp, Warning, TEXT("ASpectator Destroyed"));
}

//My methods

void ASpectator::move_right(float value) {
	//FVector force_to_add = movement_force * value * FVector(0, 1, 0);
	//auto root = (USceneComponent*)RootComponent;
	//root->AddLocalOffset(force_to_add);
	should_move_left = -value;
}

void ASpectator::move_up(float value) {
	//FVector force_to_add = movement_force * value * FVector(1, 0, 0);
	//auto root = (USceneComponent*)RootComponent;
	//root->AddLocalOffset(force_to_add);
	should_move_up = value;
}

void ASpectator::zoom_in() {
	if (spring_arm->TargetArmLength - ZOOM_STEP > MIN_ZOOM_DIST) {
		spring_arm->TargetArmLength -= ZOOM_STEP;
	}else {
		spring_arm->TargetArmLength = MIN_ZOOM_DIST;
	}
}

void ASpectator::zoom_out() {
	if (spring_arm->TargetArmLength + ZOOM_STEP < MAX_ZOOM_DIST) {
		spring_arm->TargetArmLength += ZOOM_STEP;
	}else {
		spring_arm->TargetArmLength = MAX_ZOOM_DIST;
	}
}

bool ASpectator::build(ETurretType turret_type) {
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