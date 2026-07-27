#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UAbilityDefinition;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityChanged, FGameplayTag, AbilityTag);


UCLASS()
class GAMEPROJECT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnAbilityChanged OnAbilityChanged;
private:
	UPROPERTY()
	TObjectPtr<UAbilityDefinition> CurrentAbility;
	
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	UAbilityDefinition* GetCurrentAbility() const { return CurrentAbility; }
protected:
	
	void MovementHandler(const FInputActionValue& InputActionValue);
	void LookHandler(const FInputActionValue& InputActionValue);
	void JumpHandler(const FInputActionValue& InputActionValue);
	void StartSprintHandler();
	void InteractHandler();
	void PrimaryActionHandler();
	void SecondaryActionHandler();
	void SetCurrentAbility(UAbilityDefinition* NewAbility);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Player;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_PlayerMove;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_PlayerLook;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_PlayerJump;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_PlayerSprint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_PlayerInteract;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_PlayerPrimaryAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_PlayerSecondaryAction;
};
