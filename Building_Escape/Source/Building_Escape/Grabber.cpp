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

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else
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
		UE_LOG(LogTemp, Warning, TEXT("%s found an Input Component"), *GetOwner()->GetName());
		/// Bind the input action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing an Input Component"), *GetOwner()->GetName());
	}
}



void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));
	
	// TODO implement grabbing of objects
	GetFirstPhysicsBodyInReach();

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));

	// TODO release grabbed object

}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get Player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	/// Draw a red trace in world to visualize
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

	//DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 5.f);

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Ray-Cast (aka line trace) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);


	/// See what object is hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()));
	}
	return FHitResult();
}

