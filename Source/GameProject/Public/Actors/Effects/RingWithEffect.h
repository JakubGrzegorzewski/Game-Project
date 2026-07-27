#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Structs/StatusEffectSpec.h"
#include "RingWithEffect.generated.h"

struct FStatusEffectSpec;
class UNiagaraSystem;
class ABaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRingDestroyed);


UCLASS()
class GAMEPROJECT_API ARingWithEffect : public AActor
{
	GENERATED_BODY()

public:
	ARingWithEffect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ring")
	float Radius = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ring")
	float LifeTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ring")
	TArray<FStatusEffectSpec> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ring")
	TObjectPtr<UNiagaraSystem> NiagaraSystemAsset;

	UPROPERTY(BlueprintAssignable, Category = "Ring")
	FOnRingDestroyed OnRingDestroyed;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DestroyRing();

	UPROPERTY()
	TObjectPtr<USphereComponent> SphereComponent;
};
