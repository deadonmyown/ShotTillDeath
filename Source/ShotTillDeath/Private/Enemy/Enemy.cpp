#include "Enemy/Enemy.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	bCanTakeItem = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

