// Copyright Luca Pedrelli 2020.


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h" // GetMass
#include "Engine/World.h" // GetWorld()
#include "GameFramework/PlayerController.h" // APlayerController
#include "GameFramework/Actor.h"

// Macro to make clear where we define out parameters
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
 }


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialAngle = GetOwner()->GetActorRotation().Yaw;
	CurrentAngle = InitialAngle;
	OpenAngle += InitialAngle; // So that the door opens 90 degrees regardless of its angle with the world axes

	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component but no PressurePlate is set!"), *GetOwner()->GetName());
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Polling, because we check each frame
	// OpenDoor if mass in the volume is enough
	if (TotalMassOfActors() > MassToOpen)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	// If mass isn't enough anymore, the door starts closing some seconds
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

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors; // Find all overlapping actors
	if(!PressurePlate)
		return TotalMass;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	// Add up the actors masses
	for (AActor* OverlappingActor : OverlappingActors)
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

	return TotalMass;
}