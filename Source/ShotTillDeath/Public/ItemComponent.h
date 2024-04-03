#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"

class AShotTillDeathCharacter;
struct FEnhancedInputActionEventBinding;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOTTILLDEATH_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category="Item")
	FOnUseItem OnUseItem;

	/** Item offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector ItemOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* ItemMappingContext;

	/** Use Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* UseAction;

	/** Drop Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* DropAction;

	

	UFUNCTION(BlueprintCallable, Category="Item")
	void AttachItem(AShotTillDeathCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category="Item")
	void DetachItem();

	UFUNCTION(BlueprintCallable, Category="Item")
	void DropItem();

	UFUNCTION(BlueprintCallable, Category="Item")
	void UseItem();

	UFUNCTION(BlueprintCallable, Category="Item")
	bool HasItemInterface();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item", meta=(AllowPrivateAccess))
	AShotTillDeathCharacter* Character;
};
