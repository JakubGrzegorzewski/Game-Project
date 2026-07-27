// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Effects/ProjectileWithEffect.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StatusEffectComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileWithEffect::AProjectileWithEffect(){
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetSphereRadius(15.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = Speed;
	MovementComponent->MaxSpeed = Speed;
	MovementComponent->ProjectileGravityScale = 0.f;
	MovementComponent->bRotationFollowsVelocity = true;
}

void AProjectileWithEffect::BeginPlay(){
	Super::BeginPlay();

	MovementComponent->InitialSpeed = Speed;
	MovementComponent->MaxSpeed = Speed;
	MovementComponent->Velocity = GetActorForwardVector() * Speed;
	StartLocation = GetActorLocation();

	if (AActor* MyOwner = GetOwner()){
		CollisionComponent->IgnoreActorWhenMoving(MyOwner, true);
	}
	
	if (NiagaraSystemAsset){
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystemAsset, RootComponent, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		if (NiagaraComp){
			NiagaraComp->SetAutoDestroy(true);
		}
	}

	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileWithEffect::OnHit);
	GetWorld()->GetTimerManager().SetTimer(DistanceCheckTimerHandle, this, &AProjectileWithEffect::CheckDistance, 0.05f, true);

}

void AProjectileWithEffect::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
		return;

	if (Cast<AProjectileWithEffect>(OtherActor))
		return;
	
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor)){
		if (UStatusEffectComponent* TargetStatus = Character->GetStatusEffectComponent()){
			for (const FStatusEffectSpec& Spec : Effects){
				TargetStatus->ApplyEffect(Spec, GetOwner());
			}
		}
	}

	Destroy();
}

void AProjectileWithEffect::CheckDistance(){
	FVector ActorLocation = GetActorLocation();
	if (FVector::Distance(ActorLocation, StartLocation) >= MaxDistance){
		Destroy();
	}
}