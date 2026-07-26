#include "GameProject/Public/Characters/BaseCharacter.h"

#include "Components/AbilityComponent.h"
#include "Components/StatsComponent.h"
#include "Components/StatusEffectComponent.h"


ABaseCharacter::ABaseCharacter(){
	PrimaryActorTick.bCanEverTick = true;

	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
	StatusEffectComponent = CreateDefaultSubobject<UStatusEffectComponent>(TEXT("StatusEffectComponent"));
}

void ABaseCharacter::BeginPlay(){
	Super::BeginPlay();
}


void ABaseCharacter::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
