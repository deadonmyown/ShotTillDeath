// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShotTillDeathBaseCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class SHOTTILLDEATH_API AEnemy : public AShotTillDeathBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tournament", meta=(AllowPrivateAccess))
	class AShotTillDeathCharacter* MainCharacter = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
