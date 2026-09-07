// Copyright Epic Games, Inc. All Rights Reserved.

#include "graphics_game_enginePickUpComponent.h"

Ugraphics_game_enginePickUpComponent::Ugraphics_game_enginePickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void Ugraphics_game_enginePickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &Ugraphics_game_enginePickUpComponent::OnSphereBeginOverlap);
}

void Ugraphics_game_enginePickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	Agraphics_game_engineCharacter* Character = Cast<Agraphics_game_engineCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
