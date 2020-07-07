// Copyright Luca Pedrelli 2020.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 200.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// These are called at BeginPlay
	void FindPhysicsHandle();
	void SetupInputComponent();

	// These are called at key events
	void Grab();
	void Release();
	
	// Returns the first actor within reach with a physics body
	FHitResult GetFirstPhysicsBodyInReach() const;

	FVector GetPlayerLocation() const;
	FVector GetLineTraceEnd() const;
};
