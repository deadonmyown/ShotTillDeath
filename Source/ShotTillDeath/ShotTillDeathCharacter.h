// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShotTillDeathBaseCharacter.h"

#include "Logging/LogMacros.h"
#include "ShotTillDeathCharacter.generated.h"



class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UInteractionQueueComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AShotTillDeathCharacter : public AShotTillDeathBaseCharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* MovementMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* ItemMappingContext;

	/** Use Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* UseAction;

	/** Drop Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* DropAction;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* TournamentMappingContext;

	/** Exit Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ExitAction;
	
public:
	AShotTillDeathCharacter();

protected:
	virtual void BeginPlay();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess))
	class AEnemy* Enemy = nullptr;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character")
	void EndTournamentSetupInputs();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Character")
	void StartTournamentClearInputs();

	//Interaction Queue Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"))
	UInteractionQueueComponent* InteractionQueueComponent;

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for interacting input */
	void Interact();
protected:

	/** Called for movement input */
	void Jump();

	/** Called for movement input */
	void StopJumping();
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

