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
	float InitialYaw, CurrentYaw;

	UPROPERTY(EditAnywhere) // Exposing the TargetYaw in the editor and the blueprints
	float TargetYaw = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere) // Exposing what opens our door
	AActor* ActorThatOpens;

	float DoorLastOpened = 0.f;
	float DoorCloseDelay = .5f; // Time that passes before starting to close the door

private: // Private functions
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
};
