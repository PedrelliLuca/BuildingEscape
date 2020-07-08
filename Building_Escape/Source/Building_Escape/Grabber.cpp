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
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

// Check for the PhysicsHandle component. Returns the 1st component found of type
// UPhysicsHandleComponent
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
		UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent not found on %s"),
			*GetOwner()->GetName()
		);
}

// Unreal automatically adds a hidden UInputComponent to every actor, we just need to setup it
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	
	// The string param has to be the same you setup in Project Settings>Engine>Inputs
	// the 4th parameter is a ptr to the function you want to call when the event happens
	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!PhysicsHandle)
		return;
		
	// If the physics handle is attached to something move the object we're holding
	if (PhysicsHandle->GrabbedComponent)
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
}

// Called by the InputComponent when the key(s) defined in Project Settings is pressed
void UGrabber::Grab()
{
	FHitResult Hit = GetFirstPhysicsBodyInReach();

	// If we hit something -> attach the physics handle
	if (Hit.GetActor())
	{
		UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();
		if (!PhysicsHandle)
			return;
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None, // We don't have any skeletal mesh, this is the NONE value for a FName
			GetLineTraceEnd()
		);
	}
}

// Called by the InputComponent when the key(s) defined in Project Settings is released
void UGrabber::Release()
{
	if (!PhysicsHandle)
		return;
	// Detach the physics handle
	PhysicsHandle->ReleaseComponent();
}

// Returns the first actor within reach with a physics body channel. 
// - An actor is in reach if the line trace crosses its collision box
// - An actor has a physics body channel if physics is turned on for it
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerLocation(), // Position where the line trace begins
		GetLineTraceEnd(),  // Position where the line trace ends
		// Only meshes with PhysicsBody channel are detected
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParams
	);
	return Hit;
}

// Returns the position of the line trace end with respect to world origin. 
// The line trace is the vector with:
// - origin on the player position with respect to world origin (viewpoint location), 
// - direction equal to where the player is looking (viewpoint rotation)
// - lenght defined by the Reach attribute.
FVector UGrabber::GetLineTraceEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get player's viewpoint, i.e. the location and rotation of the player pawn with respect to
	// the world origin
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// Formula to get the position of the line trace end with respect to world origin
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

// Returns the ViewPoint location, which is also the LineTraceBegin
FVector UGrabber::GetPlayerLocation() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get player's viewpoint, i.e. the location and rotation of the player pawn with respect to
	// the world origin
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}
