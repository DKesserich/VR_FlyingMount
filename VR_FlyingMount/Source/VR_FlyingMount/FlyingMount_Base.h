// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "FlyingMount_Base.generated.h"

UCLASS(Config=FlyingMountConfig, DefaultConfig)
class VR_FLYINGMOUNT_API AFlyingMount_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlyingMount_Base();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, EditDefaultsOnly)
	USphereComponent* Root;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Handle;

	//The maximum speed that we are allowed to fly. Set in DefaultFlyingMountConfig.ini
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Config)
	float MaxFlySpeed;
	
	//The maximum speed that we are allowed to turn. Set in DefaultFlyingMountConfig.ini
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Config)
	float MaxTurnSpeed;

	//The Hand that is currently holding the handle.
	UPROPERTY(BlueprintReadWrite)
	USceneComponent* HoldingHand;

	//Setter function for CanRoll
	UFUNCTION(BlueprintCallable)
	void SetCanRoll(bool Enabled) { CanRoll = Enabled; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_TargetTransform)
	FTransform TargetTransform;

	//Whether we can roll or not.
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintSetter = SetCanRoll)
	bool CanRoll = true;

	UFUNCTION(Server, Unreliable)
	void Server_UpdateTargetTransform(FTransform NewTargetTransform);
	void Server_UpdateTargetTransform_Implementation(FTransform NewTargetTransform);

	UFUNCTION()
	void OnRep_TargetTransform();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called from Blueprint. Gets the current value of Throttle from the owning Pawn.
	UFUNCTION(BlueprintImplementableEvent)
	float GetThrottleValue();

private:

	float EasedThrottle = 0;

};
