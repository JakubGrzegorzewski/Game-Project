#include "GameProject/Public/Characters/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/AbilityComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter(){
	PrimaryActorTick.bCanEverTick = true;

	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));	
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void APlayerCharacter::BeginPlay(){
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	FVector InputVector = GetLastMovementInputVector();

	if (!InputVector.IsNearlyZero()){
		float ForwardDot = FVector::DotProduct(CameraComponent->GetForwardVector(), InputVector.GetSafeNormal());

		if (ForwardDot > -0.2f) {
			FRotator TargetRotation = InputVector.Rotation();
			FRotator CurrentRotation = GetActorRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);

			SetActorRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!PlayerInputComponent) return;
	
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (IA_PlayerMove) EnhancedInputComponent->BindAction(IA_PlayerMove, ETriggerEvent::Triggered, this, &APlayerCharacter::MovementHandler);
		if (IA_PlayerLook) EnhancedInputComponent->BindAction(IA_PlayerLook, ETriggerEvent::Triggered, this, &APlayerCharacter::LookHandler);
		if (IA_PlayerJump) EnhancedInputComponent->BindAction(IA_PlayerJump, ETriggerEvent::Started, this, &APlayerCharacter::JumpHandler);
		if (IA_PlayerInteract) EnhancedInputComponent->BindAction(IA_PlayerInteract, ETriggerEvent::Triggered, this, &APlayerCharacter::InteractHandler);
		if (IA_PlayerPrimaryAction) EnhancedInputComponent->BindAction(IA_PlayerPrimaryAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PrimaryActionHandler);
		if (IA_PlayerSecondaryAction) EnhancedInputComponent->BindAction(IA_PlayerSecondaryAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SecondaryActionHandler);
		
		
		if (IMC_Player) {
			if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
					Subsystem->AddMappingContext(IMC_Player, 0);
				}
			}
		}
	}
	
	
}

void APlayerCharacter::MovementHandler(const FInputActionValue& InputActionValue){
	if (InputActionValue.GetValueType() != EInputActionValueType::Axis2D) return;
	FVector2D MovementVector = InputActionValue.Get<FVector2D>(); 
	if (MovementVector.Y > 0.f)
			AddMovementInput(CameraComponent->GetForwardVector(), MovementVector.Y);
	AddMovementInput(CameraComponent->GetRightVector(), MovementVector.X);
}
void APlayerCharacter::LookHandler(const FInputActionValue& InputActionValue){
	if (InputActionValue.GetValueType() != EInputActionValueType::Axis2D) return;
	const FVector2D LookValue = InputActionValue.Get<FVector2D>();
	SpringArmComponent->AddLocalRotation(FRotator(LookValue.Y, LookValue.X, 0));
	SpringArmComponent->SetRelativeRotation(FRotator(FMath::ClampAngle(SpringArmComponent->GetRelativeRotation().Pitch, -25, 25), SpringArmComponent->GetRelativeRotation().Yaw, 0));
}

void APlayerCharacter::JumpHandler(const FInputActionValue& InputActionValue){

}

void APlayerCharacter::InteractHandler(){

}

void APlayerCharacter::PrimaryActionHandler(){
	if (!CurrentAbility) return;
	GetAbilityComponent()->TryUseAbility(CurrentAbility->AbilityTag, this);
}

void APlayerCharacter::SecondaryActionHandler(){
	int32 INT32 = GetAbilityComponent()->GrantedAbilities.Find(CurrentAbility);
	if (TObjectPtr<UAbilityDefinition> AbilityDefinition = GetAbilityComponent()->GrantedAbilities[INT32+1])
	{
		CurrentAbility = AbilityDefinition;
	}
	else
	{
		CurrentAbility = GetAbilityComponent()->GrantedAbilities[0];
	}
	
}
