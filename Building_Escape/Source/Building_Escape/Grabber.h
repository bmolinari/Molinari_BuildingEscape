// Copyright Brendan Molinari 2017

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:

	UPROPERTY(EditAnywhere)
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray-Cast and grab object in reach
	void Grab();

	// Release the currently grabbed object
	void Release();

	// Finds the Physics Handle component, displays error if component is missing
	void FindPhysicsHandleComponent();
	
	// Sets up the Input Component, displays an error if input component is missing
	void SetupInputComponent();

	// Get the current start of the reach line
	FVector GetReachLineStart();
	
	// Get the current end point of the reach line
	FVector GetReachLineEnd();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
};
