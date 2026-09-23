// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "graphics_game_engineProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UProjectileMovementComponent;
class UMaterialInterface;
class UNiagaraSystem;

UCLASS(config=Game)
class Agraphics_game_engineProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ballMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* baseMat;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* colorP;




protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Projectile Material")
	UMaterialInterface* projMat;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* dmiMat;

	FLinearColor randColor;

public:
	Agraphics_game_engineProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

