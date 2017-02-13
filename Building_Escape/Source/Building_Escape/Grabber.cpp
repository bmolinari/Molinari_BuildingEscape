// Copyright Brendan Molinari 2017

#include "Building_Escape.h"
#include "Grabber.h"

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
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing a Physics Handle Component"), *GetOwner()->GetName());
	}
}

/// Look for attached Input Component (appears at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing an Input Component"), *GetOwner()->GetName());
	}
}


/// Grabs a valid physics object
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));

	/// Line Trace to see if we hit any valid objects to be grabbed
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	/// If we hit a valid object, attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

/// Releases the currently grabbed object
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));

	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

/// Gets the starting location of the Reach Line
FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

/// Gets the end point of the Reach Line
FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}

/// Gets and returns the first physics body hit that is in reach of the player by ray-casting.
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Ray-Cast (aka line trace) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, GetReachLineStart(), GetReachLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	/// See what object is hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()));
	}
	return HitResult;
}

