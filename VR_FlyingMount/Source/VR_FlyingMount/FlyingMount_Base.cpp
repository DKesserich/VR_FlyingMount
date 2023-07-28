// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingMount_Base.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFlyingMount_Base::AFlyingMount_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Root = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	Root->SetEnableGravity(false);

	RootComponent = Root;

	Handle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));

	Handle->SetupAttachment(Root);

}

void AFlyingMount_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlyingMount_Base, Root);
	DOREPLIFETIME(AFlyingMount_Base, TargetTransform);
	DOREPLIFETIME(AFlyingMount_Base, MaxFlySpeed);
	DOREPLIFETIME(AFlyingMount_Base, MaxTurnSpeed);
	DOREPLIFETIME(AFlyingMount_Base, CanRoll);
	DOREPLIFETIME(AFlyingMount_Base, HoldingHand);
}

// Called when the game starts or when spawned
void AFlyingMount_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlyingMount_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	
	if (GetOwner() && Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		UpdateMovement(TargetTransform, EasedThrottle, DeltaTime, GetThrottleValue());
		
		//Send our new target transform to the server.
		Server_UpdateTargetTransform(TargetTransform);
	}

}

void AFlyingMount_Base::UpdateMovement(FTransform& UpdatedTransform, float& ThrottleEased, float DeltaTime, float Throttle) const
{	
	//If the handle is being held.
	if (HoldingHand)
	{
		//Ease the throttle value so we don't instantly transition from stop to start or vice versa
		ThrottleEased = FMath::Lerp(ThrottleEased, Throttle, DeltaTime);

		//Get the location we want to be based on the forward vector of the Handle, our maximum flying speed, and the throttle
		UpdatedTransform.SetLocation(GetActorLocation() + (Handle->GetForwardVector() * (MaxFlySpeed * ThrottleEased)));

		//Convert the location of the hand holding the handle to our space
		FVector handPosition = GetActorTransform().InverseTransformPosition(HoldingHand->GetComponentLocation());

		//Rotate the handle to point at the holding hand
		FRotator handleRotation = UKismetMathLibrary::FindLookAtRotation(Handle->GetRelativeLocation(), handPosition);
		handleRotation.Yaw = FMath::Clamp(handleRotation.Yaw, -20, 20);
		handleRotation.Pitch = FMath::Clamp(handleRotation.Pitch, -20, 20);

		Handle->SetRelativeRotation(handleRotation);

		//Combine the current rotation of the actor with the rotation of the handle. Use Quaternions so we can do loop-de-loops
		UpdatedTransform.SetRotation((GetActorTransform().GetRotation() * Handle->GetRelativeTransform().GetRotation()));
	}
	else
	{
		//If we don't have a holding hand, ease the handle back to starting position.
		Handle->SetRelativeRotation(FQuat::Slerp(Handle->GetRelativeRotation().Quaternion(), FRotator::ZeroRotator.Quaternion(), DeltaTime));

		//Zero out the pitch and roll.
		FRotator UnRolled = GetActorRotation();

		UnRolled.Roll = 0;

		UnRolled.Pitch = 0;

		UpdatedTransform.SetRotation(UnRolled.Quaternion());

		//Ease back to stop
		ThrottleEased = FMath::Lerp(ThrottleEased, 0, DeltaTime);

		UpdatedTransform.SetLocation(GetActorLocation() + (Handle->GetForwardVector() * (MaxFlySpeed * ThrottleEased)));

		
	}
	
}

void AFlyingMount_Base::Server_UpdateTargetTransform_Implementation(FTransform UpdatedTransform)
{
	//Set the replicated TargetTransform variable to the received NewTargetTransform. Call OnRep so the Server does the same math as any clients.
	TargetTransform = UpdatedTransform;

	OnRep_TargetTransform();
}

void AFlyingMount_Base::OnRep_TargetTransform()
{
	//Derive a linear velocity from our current location and the TargetTransform location and set our physics to it.
	FVector LinearVelocity = TargetTransform.GetLocation() - GetActorLocation();

	Root->SetPhysicsLinearVelocity(LinearVelocity);

	//If we can't roll, zero out the roll of TargetTransform's rotation.
	if (!CanRoll)
	{
		FRotator UnRolled = TargetTransform.GetRotation().Rotator();

		UnRolled.Roll = 0;

		TargetTransform.SetRotation(UnRolled.Quaternion());
	}

	//Derive angular velocities for Pitch, Yaw, and Roll from the cross products of the forward and up vectors.

	FVector AngularVelocity = GetActorForwardVector().Cross(TargetTransform.GetRotation().GetForwardVector()) * MaxTurnSpeed * GetWorld()->DeltaTimeSeconds;

	FVector RollVelocity = GetActorUpVector().Cross(TargetTransform.GetRotation().GetUpVector()) * MaxTurnSpeed * GetWorld()->DeltaTimeSeconds;

	Root->SetPhysicsAngularVelocityInRadians(AngularVelocity + RollVelocity);
	
}

