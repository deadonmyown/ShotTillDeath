#include "Tournament/ShopHolder.h"
#include "PickupActor.h"
AShopHolder::AShopHolder()
{
	ShopRootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(ToRawPtr(ShopRootComponent));
	
	MaxItemsCount = 4;
	TransformsForItems.Init(FTransform::Identity, MaxItemsCount);
	Items.Init(nullptr, MaxItemsCount);
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

void AShopHolder::RemoveItem(APickupActor* Item)
{
	const int32 ItemIndex = Items.IndexOfByKey(Item);
	if(ItemIndex != INDEX_NONE)
	{
		Items[ItemIndex] = nullptr;
	}
}

void AShopHolder::AddItem(TSubclassOf<APickupActor> ItemClass)
{
	if(!GetWorld())
	{
		return;
	}
	
	int32 FreeSlotIndex;
	if(IsShopHolderFull(FreeSlotIndex))
	{
		return;
	}
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	
	APickupActor* Item = GetWorld()->SpawnActor<APickupActor>(ItemClass, TransformsForItems[FreeSlotIndex], SpawnInfo);
	
	Item->SetReturnOnDefaultLocation(true);
	Item->SetDefaultTransform(TransformsForItems[FreeSlotIndex]);
	
	Item->OnUseItem.AddDynamic(this, &AShopHolder::OnUseItem);
	Items[FreeSlotIndex] = Item;
}

bool AShopHolder::IsShopHolderFull(int32& FreeSlotIndex)
{
	int32 Counter = 0;
	FreeSlotIndex = -1;
	for(int i = 0; i < Items.Num(); i++)
	{
		if(IsValid(Items[i]))
		{
			Counter++;
		}
		else if(FreeSlotIndex == -1)
		{
			FreeSlotIndex = i;
		}
	}

	return Counter >= MaxItemsCount;
}

int32 AShopHolder::FreeSlotsCount()
{
	int32 Counter = 0;
	for(int i = 0; i < Items.Num(); i++)
	{
		if(!IsValid(Items[i]))
		{
			Counter++;
		}
	}
	return Counter;
}

int32 AShopHolder::FindFirstItem()
{
	for(int i = 0; i < Items.Num(); i++)
	{
		if(IsValid(Items[i]))
		{
			return i;
		}
	}
	return -1;
}

void AShopHolder::OnUseItem(APickupActor* PickupActor, bool IsUseItemSuccess)
{
	if(!IsUseItemSuccess || !GetWorld())
	{
		return;
	}

	PickupActor->bCanUse = false;
	FTimerDelegate DestroyDelegate = FTimerDelegate::CreateUObject(this, &AShopHolder::DestroyItem, PickupActor);
	GetWorldTimerManager().SetTimer(DestroyTimer, DestroyDelegate, 1.0f, false);
}

void AShopHolder::DestroyItem(APickupActor* PickupActor)
{
	PickupActor->DropItem();
	RemoveItem(PickupActor);
	GetWorld()->DestroyActor(PickupActor);
}


void AShopHolder::SetLocationsForItems()
{
	for(int i = 0; i < LocationsComponent.Num(); i++)
	{
		TransformsForItems[i] = LocationsComponent[i]->GetComponentTransform();
	}
}

void AShopHolder::ReinitializeComponents()
{
	TransformsForItems.Reset();

	for(int i = 0; i < LocationsComponent.Num(); i++)
	{
		LocationsComponent[i]->DestroyComponent();
	}
	
	TransformsForItems.Init(FTransform::Identity, 4);
	for(int i = 0; i < MaxItemsCount; i++)
	{
		LocationsComponent.Add(CreateDefaultSubobject<USceneComponent>(*FString("LocationComponent" + FString::FromInt(i))));
	}
}

bool AShopHolder::TryTakeRandomItem(AShotTillDeathBaseCharacter* OtherCharacter)
{
	int32 FirstIndex = FindFirstItem();
	if(FirstIndex == -1)
	{
		return false;
	}
	auto RandItem = Items[FirstIndex];
	RandItem->TakePickupItem(OtherCharacter);
	return true;
}