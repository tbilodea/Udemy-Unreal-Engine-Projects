// Copyright Bilodeau 2020
#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

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
	SetupPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s did not find a UPhysicsHandleComponent"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s did not find a UInputComponent"), *GetOwner()->GetName());
	}
	else
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

void UGrabber::Grab()
{
	FHitResult Hit = GetFirstPhysicsBodyInReach();

	if (Hit.GetActor())
	{
		if (PhysicsHandle)
		{
			UPrimitiveComponent *ComponentToGrab = Hit.GetComponent();
			PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayerReach());
		}
	}
}

FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	//Get a player viewport
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	//Ray cast distance
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewPointLocation = GetPlayerLocation();

	//Ray cast distance
	FVector LineTraceEnd = GetPlayerReach();

	//See what it hits
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	return Hit;
}

void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

FVector UGrabber::GetPlayerLocation() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	//Get a player viewport
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}
