// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OptionPtr.h"

enum class ETurretType : uint8;

#include "BuildSpot.generated.h"

//TODO may be namespace?
const float BUILDSPOT_CLICK_RADUS = 100.0;

UCLASS()
class UE_TEST1_API ABuildSpot : public AActor
{
	GENERATED_BODY()
//UE variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* decal;

	//TODO is it static?
	UPROPERTY(EditDefaultsOnly, Category = Turret)
	TSubclassOf<class ATurret> Turret1;

	UPROPERTY(EditDefaultsOnly, Category = Turret)
	TSubclassOf<class ATurret> Turret2;

	UPROPERTY(EditDefaultsOnly, Category = Turret)
	TSubclassOf<class ATurret> Turret3;
public:
//UE events and methods
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Reset() override;
public:	
	// Sets default values for this actor's properties
	ABuildSpot();
//My variables
private:
	OptionPtr<ATurret> turret;

//My methods
private:
	OptionPtr<UWorld> get_world();
	bool spawn_turret(UWorld& world, TSubclassOf<class ATurret>& turret, FVector& pos);
public:	
	bool build(ETurretType turret_type);
	void on_turret_destoyed(class ATurret& turret);
};
