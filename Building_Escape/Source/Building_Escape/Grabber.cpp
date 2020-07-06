// Copyright Luca Pedrelli 2020.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h" // GetWorld()
#include "GameFramework/PlayerController.h" // For viewport

// Macro to make clear where we define out parameters
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Check for the PhysicsHandle component. Returns the 1st component found
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
		UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent not found on %s"),
			*GetOwner()->GetName()
		);
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	
	// UE_LOG(LogTemp, Warning, TEXT("ViewPointLocation: %s ViewPointRotation: %s"),
	// 	*PlayerViewPointLocation.ToString(),
 	// 	*PlayerViewPointRotation.ToString()
	// );

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// To visually test our LineTraceEnd
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);

	// Ray casting
	FHitResult Hit;
	// We're not using tags in order to query whether we've collided with something (trace tag), we
	// can therefore leave FName empty.
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		// Only meshes with PhysicsBody channel are detected
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParams
	);
	
	// Logging the actor we hit
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit) // Jesus please protect me from nullptrs
		UE_LOG(LogTemp, Warning, TEXT("Hit actor %s!"), *ActorHit->GetName());
}

