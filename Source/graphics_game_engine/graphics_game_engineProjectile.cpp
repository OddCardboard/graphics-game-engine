// Copyright Epic Games, Inc. All Rights Reserved.

#include "graphics_game_engineProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

Agraphics_game_engineProjectile::Agraphics_game_engineProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &Agraphics_game_engineProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	ballMesh->SetupAttachment(CollisionComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void Agraphics_game_engineProjectile::BeginPlay()
{
	Super::BeginPlay();

	randColor = FLinearColor(
		UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f),
		UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f),
		UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f),
		1.0f
	);

	if (IsValid(projMat) && IsValid(ballMesh))
	{
		dmiMat = UMaterialInstanceDynamic::Create(
			projMat,
			this
		);

		if (IsValid(dmiMat))
		{
			ballMesh->SetMaterial(0, dmiMat);

			dmiMat->SetVectorParameterValue(
				TEXT("ProjColor"),
				randColor
			);
		}
	}
}

void Agraphics_game_engineProjectile::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	// Stop if the projectile did not hit a valid actor
	if (!IsValid(OtherActor) || OtherActor == this)
	{
		return;
	}

	// Add an impulse if the hit object uses physics
	if (IsValid(OtherComp) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(
			GetVelocity() * 100.0f,
			Hit.ImpactPoint
		);
	}

	// Make sure the decal material and world are valid
	if (IsValid(baseMat) && IsValid(GetWorld()))
	{

		float frameNum =
			static_cast<float>(FMath::RandRange(0, 3));

		float decalSize =
			UKismetMathLibrary::RandomFloatInRange(20.0f, 40.0f);


		// Create the dynamic material before spawning the decal
		UMaterialInstanceDynamic* MatInstance =
			UMaterialInstanceDynamic::Create(
				baseMat,
				GetWorld()
			);

		if (IsValid(MatInstance))
		{

			if (IsValid(colorP))
			{
				UNiagaraComponent* particleComp =
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						GetWorld(),
						colorP,
						Hit.ImpactPoint + (Hit.ImpactNormal * 5.0f),
						Hit.ImpactNormal.Rotation(),
						FVector(1.0f),
						true,   // Auto destroy
						false   // Do not activate yet
					);

				if (IsValid(particleComp))
				{
					particleComp->SetVariableLinearColor(
						FName(TEXT("User.RandomColor")),
						randColor
					);

					particleComp->Activate(true);
				}
			}

			MatInstance->SetVectorParameterValue(
				TEXT("Color"),
				randColor
			);

			MatInstance->SetScalarParameterValue(
				TEXT("Frame"),
				frameNum
			);

			// Rotate the decal so it projects onto the hit surface
			FRotator decalRotation =
				Hit.ImpactNormal.Rotation();

			UDecalComponent* Decal =
				UGameplayStatics::SpawnDecalAtLocation(
					GetWorld(),
					MatInstance,
					FVector(decalSize),
					Hit.ImpactPoint,
					decalRotation,
					10.0f
				);

			if (!IsValid(Decal))
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("The decal failed to spawn.")
				);
			}
		}
	}

	// Destroy the projectile after the decal has been spawned
	Destroy();
}
