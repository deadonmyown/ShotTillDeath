#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "InteractionQueueComponent.h"
#include "GameFramework/Actor.h"
#include "TournamentManager.generated.h"

class AShotTillDeathCharacter;
class UInteractionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTournamentStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTournamentEnd);

UCLASS()
class SHOTTILLDEATH_API ATournamentManager : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ATournamentManager();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	
	/** Exit Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ExitAction;

	/** Exit Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* TournamentMappingContext;

	UPROPERTY(BlueprintAssignable, Category="Tournament")
	FOnTournamentStart OnTournamentStart;

	UPROPERTY(BlueprintAssignable, Category="Tournament")
	FOnTournamentEnd OnTournamentEnd;
	
	UFUNCTION(BlueprintGetter, Category="Tournament")
	FInteractionData GetInteractionData() const;

	UFUNCTION(BlueprintSetter, Category="Tournament")
	void SetInteractionData(const FInteractionData& Value);

	UFUNCTION(BlueprintCallable, Category="Tournament")
	void EnableInteraction();

	UFUNCTION(BlueprintCallable, Category="Tournament")
	void DisableInteraction();

	UFUNCTION(BlueprintCallable, Category="Tournament")
	bool StartTournament(AShotTillDeathCharacter* OtherCharacter);

	UFUNCTION(BlueprintCallable, Category="Tournament")
	void EndTournament();

	UFUNCTION(BlueprintCallable, Category="Tournament")
	void SetupEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tournament")
	FVector MainCharacterPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tournament")
	FRotator MainCharacterRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tournament")
	FVector EnemyPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tournament")
	FRotator EnemyRotation;

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Tournament")
	void SetCurrentPositionForMainCharacter();

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Tournament")
	void SetCurrentPositionForEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> MainCharacterTransformComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> EnemyTransformComponent = nullptr;

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Tournament")
	void SetCharacterTurn(class AShotTillDeathBaseCharacter* TurnCharacter);

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Tournament")
	void ResetCharacterTurn();

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Tournament")
	void SwapCharacterTurn();
	
protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UInteractionComponent> InteractionTriggerComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetInteractionData, BlueprintSetter=SetInteractionData,
		Category="Tournament")
	FInteractionData InteractionData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess))
	TObjectPtr<USceneComponent> TournamentRootComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tournament")
	TSubclassOf<class AEnemy> TypeOfEnemy;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tournament", meta=(AllowPrivateAccess))
	AShotTillDeathCharacter* TournamentMainCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tournament", meta=(AllowPrivateAccess))
	AEnemy* TournamentEnemy = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tournament", meta=(AllowPrivateAccess))
	AShotTillDeathBaseCharacter* CurrentCharacter = nullptr;
	
	virtual bool FinishInteraction_Implementation(AActor* OtherActor) override;

	struct FEnhancedInputActionEventBinding* LookActionEventBinding = nullptr;

	FEnhancedInputActionEventBinding* InteractActionEventBinding = nullptr;

	FEnhancedInputActionEventBinding* ExitActionEventBinding = nullptr;
};
