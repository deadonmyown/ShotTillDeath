#include "ShotTillDeathBaseCharacter.h"
#include "Components/CapsuleComponent.h"

AShotTillDeathBaseCharacter::AShotTillDeathBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	/*// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(RootComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;*/
}

void AShotTillDeathBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AShotTillDeathBaseCharacter::ChangeCharacterState(ECharacterState NewCharacterState)
{
	CharacterState = NewCharacterState;
}

void AShotTillDeathBaseCharacter::SetHasItem(bool bNewHasItem)
{
	bHasItem = bNewHasItem;
}

bool AShotTillDeathBaseCharacter::GetHasItem()
{
	return bHasItem;
}

