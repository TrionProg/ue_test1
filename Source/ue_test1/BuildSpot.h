// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.h"
#include "OptionPtr.h"

#include "BuildSpot.generated.h"

//TODO may be namespace?
const float BUILDSPOT_CLICK_RADUS = 100.0;

UCLASS()
class UE_TEST1_API ABuildSpot : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* decal;

	//TODO is it static?
	UPROPERTY(EditDefaultsOnly, Category = Turret)
	TSubclassOf<class ATurret> Turret1;

	UPROPERTY(EditDefaultsOnly, Category = Turret)
	TSubclassOf<class ATurret> Turret2;

	UPROPERTY(EditDefaultsOnly, Category = Turret)
	TSubclassOf<class ATurret> Turret3;

	OptionPtr<ATurret> turret;
public:
	
public:	
	// Sets default values for this actor's properties
	ABuildSpot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool build(uint8 turret_type);

	void Reset();
	
};
