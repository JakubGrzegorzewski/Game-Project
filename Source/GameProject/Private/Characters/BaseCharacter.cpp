#include "GameProject/Public/Characters/BaseCharacter.h"

#include "Components/StatsComponent.h"


ABaseCharacter::ABaseCharacter(){
	PrimaryActorTick.bCanEverTick = true;
	
	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
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
