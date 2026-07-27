#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "UI/BaseTopBarWidget.h"
#include "BaseCharacter.generated.h"

class UStatsComponent;
class UAbilityComponent;
class UStatusEffectComponent;
class UCharacterAttributeSet;

UCLASS()
class GAMEPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStatsComponent* StatsComponent;
	UStatsComponent* GetStatsComponent() const { return StatsComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAbilityComponent* AbilityComponent;
	UAbilityComponent* GetAbilityComponent() const { return AbilityComponent; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStatusEffectComponent* StatusEffectComponent;
	UStatusEffectComponent* GetStatusEffectComponent() const { return StatusEffectComponent; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UWidgetComponent* TopBarWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UBaseTopBarWidget> TopBarWidgetClass;
	

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
