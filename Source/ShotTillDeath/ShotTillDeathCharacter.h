// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ShotTillDeathCharacter.generated.h"

UENUM(BlueprintType)
enum ECharacterState
{
	Default = 0 UMETA(DisplayName = "Default"),
	InShop = 1 UMETA(DisplayName = "In Shop"),
	InTournament = 2 UMETA(DisplayName = "In Tournament")
};

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UInteractionQueueComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AShotTillDeathCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	AShotTillDeathCharacter();

protected:
	virtual void BeginPlay();

public:

	UFUNCTION(BlueprintCallable, Category=Input)
	void SetupPlayerInputs();

	UFUNCTION(BlueprintCallable, Category=Input)
	void ClearPlayerInputs();

	//Interaction Queue Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction", meta = (AllowPrivateAccess = "true"))
	UInteractionQueueComponent* InteractionQueueComponent;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	bool bHasItem;

	UFUNCTION(BlueprintCallable, Category="Item")
	void SetHasItem(bool bNewHasItem);

	UFUNCTION(BlueprintCallable, Category="Item")
	bool GetHasItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character")
	TEnumAsByte<ECharacterState> CharacterState = ECharacterState::Default;

	UFUNCTION(BlueprintCallable, Category="Character")
	void ChangeCharacterState(ECharacterState NewCharacterState);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for interacting input */
	void Interact();
protected:
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

