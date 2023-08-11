// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "MountActor_Base.generated.h"

UCLASS(Config=FlyingMount, DefaultConfig)
class FLYINGMOUNT_API AMountActor_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMountActor_Base();

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
	UPROPERTY(Replicated, BlueprintReadWrite)
	USceneComponent* HoldingHand;

	//Setter function for CanRoll
	UFUNCTION(BlueprintCallable)
	void SetCanRoll(bool Enabled) { CanRoll = Enabled; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_TargetTransform)
	FTransform TargetTransform;

	FTransform NewTargetTransform;

	//Whether we can roll or not.
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintSetter = SetCanRoll)
	bool CanRoll = true;

	UFUNCTION(Server, Unreliable)
	void Server_UpdateTargetTransform(FTransform UpdatedTransform);
	void Server_UpdateTargetTransform_Implementation(FTransform UpdatedTransform);

	UFUNCTION()
	void OnRep_TargetTransform();

	void UpdateMovement(FTransform& UpdatedTransform, float& ThrottleEased, float DeltaTime, float Throttle) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called from Blueprint. Gets the current value of Throttle from the owning Pawn.
	UFUNCTION(BlueprintImplementableEvent)
	float GetThrottleValue();

private:

	float EasedThrottle = 0;

	//Custom LookAts for getting roll based on camera location
	FQuat GetLookAtQuaternion(const FVector& Start, const FVector& Target) const;

	FQuat MyLookAt(const FVector& lookAt, const FVector& upDirection) const;

};
