// Copyright Brendan Molinari 2017

#include "Building_Escape.h"
#include "OpenDoor.h"
#define OUT

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

	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	FRotator *DoorRotation = new FRotator(0.0f, OpenAngle, 0.0f);
	Owner->SetActorRotation(*DoorRotation);
}

void UOpenDoor::CloseDoor()
{
	FRotator *DoorRotation = new FRotator(0.0f, -90.0f, 0.0f);
	Owner->SetActorRotation(*DoorRotation);
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Check for trigger, then open door when activated
	if (GetTotalMassOnPlate() >= 30.f) // TODO make into a parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it is time to close the door after short delay of opening
	if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay)
	{
		CloseDoor();
	}
}

// Returns total mass in Kg within trigger volume
float UOpenDoor::GetTotalMassOnPlate()
{
	float TotalMass = 0.f;

	// Find all actors in trigger volume
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them and calculate total mass
	for (auto& Actor : OverlappingActors)
	{
		/*FString name = Actor->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *name);*/

		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}
