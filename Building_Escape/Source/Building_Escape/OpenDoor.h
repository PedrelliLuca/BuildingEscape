// Copyright Luca Pedrelli 2020.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h" // To add ATriggerVolume
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private: // Private attributes
	float InitialAngle, CurrentAngle;

	UPROPERTY(EditAnywhere) // Exposing the OpenAngle in the editor and the blueprints
	float OpenAngle = 90.f;

	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = .5f; // Time that passes before starting to close the door

	UPROPERTY(EditAnywhere)
	float OpeningSpeed = .8f;
	
	UPROPERTY(EditAnywhere)
	float  ClosingSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere) // Exposing what opens our door
	AActor* ActorThatOpens;

private: // Private functions
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
};
