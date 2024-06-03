#include "Tournament/ShopHolder.h"
#include "PickupActor.h"
AShopHolder::AShopHolder()
{
	ShopRootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(ToRawPtr(ShopRootComponent));
	
	MaxItemsCount = 4;
	LocationsForItems.Init(FVector::Zero(), 4);
	for(int i = 0; i < MaxItemsCount; i++)
	{
		LocationsComponent.Add(CreateDefaultSubobject<USceneComponent>(*FString("LocationComponent" + FString::FromInt(i))));
	}
}

void AShopHolder::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShopHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShopHolder::AddItem(APickupActor* Item)
{
	if(IsShopHolderFull())
	{
		return;
	}
	Items.Add(Item);
	Item->OnUseItem.AddDynamic(this, &AShopHolder::OnUseItem);
}

void AShopHolder::RemoveItem(APickupActor* Item)
{
	Items.Remove(Item);
}

void AShopHolder::SetItemLocation(APickupActor* Item)
{
	if(IsShopHolderFull())
	{
		return;
	}
	Item->SetActorLocation(LocationsForItems[Items.Num() - 1]);
}

bool AShopHolder::IsShopHolderFull()
{
	return Items.Num() >= MaxItemsCount;
}

void AShopHolder::OnUseItem(APickupActor* PickupActor, bool IsUseItemSuccess)
{
	if(!IsUseItemSuccess || !GetWorld())
	{
		return;
	}

	PickupActor->DropItem();
	RemoveItem(PickupActor);
	GetWorld()->DestroyActor(PickupActor);
}

void AShopHolder::SetLocationsForItems()
{
	for(int i = 0; i < LocationsComponent.Num(); i++)
	{
		LocationsForItems[i] = LocationsComponent[i]->GetComponentLocation();
	}
}

void AShopHolder::ReinitializeComponents()
{
	LocationsForItems.Reset();

	for(int i = 0; i < LocationsComponent.Num(); i++)
	{
		LocationsComponent[i]->DestroyComponent();
	}
	
	LocationsForItems.Init(FVector::Zero(), 4);
	for(int i = 0; i < MaxItemsCount; i++)
	{
		LocationsComponent.Add(CreateDefaultSubobject<USceneComponent>(*FString("LocationComponent" + FString::FromInt(i))));
	}
}

void AShopHolder::TryTakeRandomItem(AShotTillDeathBaseCharacter* OtherCharacter)
{
	int32 RandNum = FMath::RandRange(0, Items.Num() - 1);
	auto RandItem = Items[RandNum];
	RandItem->TakePickupItem(OtherCharacter);
}