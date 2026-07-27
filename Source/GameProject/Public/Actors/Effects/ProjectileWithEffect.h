// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Characters/BaseCharacter.h"
#include "GameFramework/Actor.h"
#include "Structs/StatusEffectSpec.h"
#include "ProjectileWithEffect.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class GAMEPROJECT_API AProjectileWithEffect : public AActor
{
	GENERATED_BODY()

public:
	AProjectileWithEffect();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Speed = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LifeTime = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MaxDistance = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TArray<FStatusEffectSpec> Effects;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UNiagaraSystem> NiagaraSystemAsset;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;
	
private:
	FVector StartLocation;
	FTimerHandle DistanceCheckTimerHandle;

	void CheckDistance();
};
