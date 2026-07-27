#include "GameProject/Public/Characters/BaseCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Components/AbilityComponent.h"
#include "Components/StatsComponent.h"
#include "Components/StatusEffectComponent.h"


ABaseCharacter::ABaseCharacter(){
	PrimaryActorTick.bCanEverTick = true;

	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
	StatusEffectComponent = CreateDefaultSubobject<UStatusEffectComponent>(TEXT("StatusEffectComponent"));
	
	TopBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("TopBarWidgetComponent"));
	TopBarWidgetComponent->SetupAttachment(GetRootComponent());
	TopBarWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	TopBarWidgetComponent->SetOwnerNoSee(true);
}

void ABaseCharacter::BeginPlay(){
	Super::BeginPlay();
	
	UBaseTopBarWidget* UserWidget = CreateWidget<UBaseTopBarWidget>( GetWorld(), TopBarWidgetClass);
	if (UserWidget){
		UserWidget->StatsComponent = StatsComponent;
		TopBarWidgetComponent->SetWidget(UserWidget);
	}
}


void ABaseCharacter::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
