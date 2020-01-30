// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicGameModeBase.h"
#include "Spectator.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"


ABasicGameModeBase::ABasicGameModeBase(): Super() {
	PrimaryActorTick.bCanEverTick = true;

	//Use C++ Spectator
	DefaultPawnClass = ASpectator::StaticClass(); 

	/*
	//Use blueprint Specrator
	static ConstructorHelpers::FClassFinder<APawn> FoundFPC(TEXT("/Game/Blueprints/SpectatorBlueprint"));
	if (FoundFPC.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found FPC"));

		DefaultPawnClass = (UClass*)FoundFPC.Class;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FPC Not found"));
		DefaultPawnClass = nullptr;
	}
	*/
}


void ABasicGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {

	UE_LOG(LogTemp, Warning, TEXT("InitGame"));

	/*

	static ConstructorHelpers::FObjectFinder<AActor> Enemy(TEXT("Blueprint'/Game/Blueprints/WeakEnemy.WeakEnemy'"));

	auto pos = FVector(800, -240, 0);

	AActor* my_actor = (AActor*)GetWorld()->SpawnActor((UClass*)Enemy.Object, &pos);
	*/

	//--------------

	static ConstructorHelpers::FClassFinder<AActor> WeakEnemyClassFinder(TEXT("Blueprint'/Game/Blueprints/WeakEnemy'"));
	if (WeakEnemyClassFinder.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("AAA"));
		//AActor* NewActor = GetWorld()->SpawnActor<AActor>(GetClass(), &position, &rotator);
		//auto WeakEnemy = Cast<TSubclassOf<AEnemy>>(WeakEnemyClassFinder.Class);
		//WeakEnemy = (AEnemy*)WeakEnemyClassFinder.Object;

		auto pos = FVector(800, -240, 0);

		AActor* my_actor = (AActor*)GetWorld()->SpawnActor(WeakEnemyClassFinder.Class, &pos);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CCC"));
		return;
	}


	/*
	static ConstructorHelpers::FObjectFinder<UClass> WeakEnemyClassFinder(TEXT("Blueprint'/Game/Blueprints/WeakEnemy.WeakEnemy'"));
	if (!WeakEnemyClassFinder.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("AAA"));
		return;
	}
	*/

	/*
	UE_LOG(LogTemp, Warning, TEXT("BBB"));

	auto pos = FVector(800, -240, 0);

	AActor* my_actor = (AActor*)GetWorld()->SpawnActor(WeakEnemy, &pos);
	*/

	/*
	WeakEnemy = WeakEnemyClassFinder.Object;

	//AFloorCell* NewFloorCell = SpawnActor<AFloorCell>(FloorCellClass, SpawnParams);

	auto pos = FVector(800, -240, 0);

	AActor* my_actor = (AActor*)GetWorld()->SpawnActor(WeakEnemy, &pos);
	*/


	/*
	static ConstructorHelpers::FObjectFinder<UBlueprint> onlineSpawnBP(TEXT("Blueprint'/Game/Blueprints/WeakEnemy.WeakEnemy'"));
	if (onlineSpawnBP)
	{
		AActor* NewActor = GetWorld()->SpawnActor<AActor>(GetClass(), &position, &rotator);
	}


	static ConstructorHelpers::FClassFinder<AActor> Enemy(TEXT("Blueprint'/Game/Blueprints/WeakEnemy.WeakEnemy'"));

	if (!Enemy.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("AAA"));
		return;
	}
	*/

	/*
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Blueprints/WeakEnemy.WeakEnemy'")));

	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	if (!SpawnActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAA"));
		return;
	}

	UClass* SpawnClass = SpawnActor->StaticClass();
	if (SpawnClass == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("BBB"));
		return;
	}

	auto pos = FVector(800, -240, 0);

	AActor* my_actor = (AActor*)GetWorld()->SpawnActor(SpawnClass, &pos);
	*/

	/*
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	*/

	Super::InitGame(MapName, Options, ErrorMessage);
}

void ABasicGameModeBase::Tick(float DeltaSeconds) {
	//UE_LOG(LogTemp, Warning, TEXT("Tick"));
}