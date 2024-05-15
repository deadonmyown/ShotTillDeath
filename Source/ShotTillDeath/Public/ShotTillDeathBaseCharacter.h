// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShotTillDeathBaseCharacter.generated.h"

UENUM(BlueprintType)
enum ECharacterState
{
	Default = 0 UMETA(DisplayName = "Default"),
	InShop = 1 UMETA(DisplayName = "In Shop"),
	InTournament = 2 UMETA(DisplayName = "In Tournament")
};

class USkeletalMeshComponent;

UCLASS()
class SHOTTILLDEATH_API AShotTillDeathBaseCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AShotTillDeathBaseCharacter();

protected:
	virtual void BeginPlay();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

public:
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

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
};
