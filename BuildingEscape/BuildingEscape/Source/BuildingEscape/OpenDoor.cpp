// Copyright Bilodeau 2020

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT
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

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw += InitialYaw;

	FindAudioComponent();
	ValidatePressurePlateSet();
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if(!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing the Audio Component"), *GetOwner()->GetName());
	}
}

void UOpenDoor::ValidatePressurePlateSet()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has a Open Door Component, but no pressure plate set."), *GetOwner()->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && TotalMassOfActors() > NeededMassForOpen)
	{
		if(!SoundPlayedOpen)
		{
			AudioComponent->Play();
			SoundPlayedClose = false;
			SoundPlayedOpen = true;
		}
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("Door opened at %f"), DoorLastOpened);
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			if(!SoundPlayedClose)
			{
				AudioComponent->Play();
				SoundPlayedClose = true;
				SoundPlayedOpen = false;
			}
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(const float &DeltaTime)
{
	FRotator ActorRotation = GetOwner()->GetActorRotation();
	ActorRotation.Yaw = FMath::FInterpTo(ActorRotation.Yaw, TargetYaw, DeltaTime, DoorOpenSpeed);
	GetOwner()->SetActorRotation(ActorRotation);
}

void UOpenDoor::CloseDoor(const float &DeltaTime)
{
	FRotator ActorRotation = GetOwner()->GetActorRotation();
	ActorRotation.Yaw = FMath::FInterpTo(ActorRotation.Yaw, InitialYaw, DeltaTime, DoorClosedSpeed);
	GetOwner()->SetActorRotation(ActorRotation);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TSet<AActor *> OverlappingActors;

	if(!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}
