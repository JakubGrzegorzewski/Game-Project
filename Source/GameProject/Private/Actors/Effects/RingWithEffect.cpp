// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Effects/RingWithEffect.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/BaseCharacter.h"
#include "Components/StatusEffectComponent.h"

ARingWithEffect::ARingWithEffect(){
	PrimaryActorTick.bCanEverTick = false;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereComponent->SetGenerateOverlapEvents(true);
	RootComponent = SphereComponent;
}

void ARingWithEffect::BeginPlay(){
	Super::BeginPlay();

	FTimerHandle LifeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(LifeTimerHandle, this, &ARingWithEffect::DestroyRing, LifeTime, false);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARingWithEffect::OnOverlapBegin);
	SphereComponent->SetSphereRadius(Radius);
	SphereComponent->UpdateOverlaps();

	if (NiagaraSystemAsset){
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraSystemAsset,
			RootComponent,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);

		if (NiagaraComp){
			NiagaraComp->SetFloatParameter(FName("User.Scale_All"), Radius / 300.f);
		}
	}
}

void ARingWithEffect::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){

	ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
	if (!Character) return;

	UStatusEffectComponent* TargetStatus = Character->GetStatusEffectComponent();
	if (!TargetStatus) return;

	for (const FStatusEffectSpec& Spec : Effects){
		TargetStatus->ApplyEffect(Spec, GetOwner());
	}
}

void ARingWithEffect::DestroyRing(){
	OnRingDestroyed.Broadcast();
	Destroy();
}
