// Copyright Bilodeau 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
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

private:
	float InitialYaw;
	float DoorLastOpened = 0.f;
	
	//Tracks whether sounds have been played
	bool SoundPlayedOpen = false;
	bool SoundPlayedClose = true;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float DoorClosedSpeed= 2.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;

	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	float NeededMassForOpen = 30.f;

	void FindAudioComponent();
	void ValidatePressurePlateSet();
	void OpenDoor(const float& DeltaTime);
	void CloseDoor(const float& DeltaTime);
	float TotalMassOfActors() const;
};
