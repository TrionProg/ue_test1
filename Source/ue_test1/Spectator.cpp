// Fill out your copyright notice in the Description page of Project Settings.

#include "Spectator.h"

#include "Runtime/Engine/Classes/Components/InputComponent.h"


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
	
}

// Called every frame
void ASpectator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent = PlayerInputComponent;

	InputComponent->BindAxis("move up", this, &ASpectator::move_up);
	InputComponent->BindAxis("move right", this, &ASpectator::move_right);
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