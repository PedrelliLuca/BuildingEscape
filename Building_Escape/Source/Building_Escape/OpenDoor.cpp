// Copyright Luca Pedrelli 2020.


#include "Engine/World.h" // GetWorld()
#include "GameFramework/PlayerController.h" // APlayerController
#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
 }


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialAngle = GetOwner()->GetActorRotation().Yaw;
	CurrentAngle = InitialAngle;
	OpenAngle += InitialAngle;

	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component but no PressurePlate is set!"), *GetOwner()->GetName());

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Polling
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	// The door starts closing some seconds after leaving the trigger volume
	else if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		CloseDoor(DeltaTime);
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator DoorRotation = GetOwner()->GetActorRotation();
 	CurrentAngle = FMath::Lerp(CurrentAngle, OpenAngle, DeltaTime * OpeningSpeed);
	DoorRotation.Yaw = CurrentAngle;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator DoorRotation = GetOwner()->GetActorRotation();
 	CurrentAngle = FMath::Lerp(CurrentAngle, InitialAngle, DeltaTime * ClosingSpeed);
	DoorRotation.Yaw = CurrentAngle;
	GetOwner()->SetActorRotation(DoorRotation);
}