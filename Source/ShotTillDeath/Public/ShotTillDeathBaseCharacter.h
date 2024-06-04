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
	InTournamentCharacterTurn = 2 UMETA(DisplayName = "In Tournament Character Turn"),
	InTournamentEnemyTurn = 3 UMETA(DisplayName = "In Tournament Enemy Turn")
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	bool bHasItem;

	UFUNCTION(BlueprintCallable, Category="Item")
	bool CanTakeItem();

	UFUNCTION(BlueprintGetter, Category="Item")
	bool GetCanTakeItem();

	UFUNCTION(BlueprintSetter, Category="Item")
	void SetCanTakeItem(bool bNewCanTake);
	
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCanTakeItem, BlueprintSetter=SetCanTakeItem, Category="Item")
	bool bCanTakeItem;

	UFUNCTION(BlueprintCallable, Category="Item")
	void SetHasItem(bool bNewHasItem);

	UFUNCTION(BlueprintCallable, Category="Item")
	bool GetHasItem();

	UFUNCTION(BlueprintCallable, Category="Item")
	void SetItem(APickupActor* Item);

	UFUNCTION(BlueprintCallable, Category="Item")
	void ResetItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta=(AllowPrivateAccess))
	class APickupActor* CurrentItem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character")
	TEnumAsByte<ECharacterState> CharacterState = ECharacterState::Default;

	UFUNCTION(BlueprintCallable, Category="Character")
	void ChangeCharacterState(ECharacterState NewCharacterState);

	UFUNCTION(BlueprintCallable, Category="Character")
	void TryUseItem();

	UFUNCTION(BlueprintCallable, Category="Character")
	AActor* GetCharacterInTournament();

	UFUNCTION(BlueprintCallable, Category="Character")
	bool GetPlayerViewport(FVector& ViewLocation, FRotator& ViewRotation);

	/** The actor caught by line of sight. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess))
	AActor* ActorInSight = nullptr;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

protected:
	/** Line of sight trace channel. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character",
		meta=(AllowPrivateAccess))
	TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	/** The line of sight max distance.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character",
		meta=(AllowPrivateAccess))
	float SightDistance = 512.f;

	/** The line of sight radius. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character",
		meta=(AllowPrivateAccess))
	float SightRadius = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character",
		meta=(AllowPrivateAccess))
	float DebugDrawTime = 0.5f;
};
