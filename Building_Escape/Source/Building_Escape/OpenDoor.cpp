// Copyright Luca Pedrelli 2020.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator DoorRotation = GetOwner()->GetActorRotation();
	UE_LOG(LogTemp, Warning, TEXT("The transform is %s.\nThe yaw is %f."), *DoorRotation.ToString(), DoorRotation.Yaw);

	// To get a door that opens slower over time
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.02f);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	// To get a door that opens at a fixed speed over time (linear interpolation)
	// DoorRotation.Yaw = FMath::FInterpConstantTo(DoorRotation.Yaw, TargetYaw, DeltaTime, 45.f);

}

