// Copyright Brendan Molinari 2017

#include "Building_Escape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	//!! First try
	/*auto Owner = GetOwner();
	Owner->SetActorRotation(*(new FQuat(0, 0, -60, 0)));*/

	//!! Second Try
	AActor *Owner = GetOwner();
	FRotator *DoorRotation = new FRotator(0.0f, -160.0f, 0.0f);
	Owner->SetActorRotation(*DoorRotation);
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Check for trigger, then open door when activated
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
	}

	
}

