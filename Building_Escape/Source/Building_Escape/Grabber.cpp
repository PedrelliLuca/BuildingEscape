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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	// Check for the PhysicsHandle component. Returns the 1st component found
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
		UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent not found on %s"),
			*GetOwner()->GetName()
		);
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{	
		// The string param has to be the same you setup in Project Settings
		// the 4th parameter is a ptr to the function you want to call when the event happens
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else 
		// It's impossible to end up in here, Unreal automatically adds a hidden UInputComponent to
		// every actor.
		UE_LOG(LogTemp, Error, TEXT("UInputComponent not found on %s"), *GetOwner()->GetName());
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If the physics handle is attach move the object we're holding
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbed!!"));
	
	// If we hit something -> attach the physics handle
	
	// Logging the actor we hit
	AActor* ActorHit = GetFirstPhysicsBodyInReach().GetActor();
	if(ActorHit) // Jesus please protect me from nullptrs
		UE_LOG(LogTemp, Warning, TEXT("Hit actor %s!"), *ActorHit->GetName());
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released!!"));

	// Detach the physics handle
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// Ray casting: Try to reach any actors with physics body collision channel set.
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		// Only meshes with PhysicsBody channel are detected
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParams
	);
	return Hit;
}
