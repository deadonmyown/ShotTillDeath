#include "Tournament/InjectorHolder.h"

#include "PickupActor.h"


AInjectorHolder::AInjectorHolder()
{
	PrimaryActorTick.bCanEverTick = true;

	InjectorRootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(ToRawPtr(InjectorRootComponent));
	
	FirstLocationComponent = CreateDefaultSubobject<USceneComponent>("FirstLocationComponent");
	LastLocationComponent = CreateDefaultSubobject<USceneComponent>("LastLocationComponent");
	ActiveLocationComponent = CreateDefaultSubobject<USceneComponent>("ActiveLocationComponent");
}

void AInjectorHolder::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInjectorHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInjectorHolder::GenerateRandomInjectors(int Count)
{
	if(!GetWorld())
	{
		return;
	}

	SetInjectorsLocations(Count);

	ResetInjectors();
	
	int MaxRandCount = TypesOfInjector.Num();
	for(int i = 0; i < Count; i++)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		
		int32 RandInjectorIndex = FMath::RandRange(0, MaxRandCount - 1);
		APickupActor* Injector = GetWorld()->SpawnActor<APickupActor>(TypesOfInjector[RandInjectorIndex], CurrentInjectorLocations[i], FRotator::ZeroRotator, SpawnInfo);
		Injector->DisablePickup();
		Injector->OnUseItem.AddDynamic(this, &AInjectorHolder::SetNewActiveInjectorOnUseItem);
		CurrentInjectors.Add(Injector);
	}
	SetNewActiveInjector();
}

void AInjectorHolder::SetInjectorsLocations(int Count)
{
	CurrentInjectorLocations.Reset();
	CurrentInjectorLocations.Add(FirstLocationForInjectors);
	FVector Offset = (LastLocationForInjectors - FirstLocationForInjectors) / Count;
	for(int i = 1; i < Count; i++)
	{
		CurrentInjectorLocations.Add(CurrentInjectorLocations[i - 1] + Offset);
	}
}

void AInjectorHolder::SetFirstPointForInjectors()
{
	if(FirstLocationComponent)
	{
		FirstLocationForInjectors = FirstLocationComponent->GetComponentLocation();
	}
}

void AInjectorHolder::SetLastPointForInjectors()
{
	if(LastLocationComponent)
	{
		LastLocationForInjectors = LastLocationComponent->GetComponentLocation();
	}
}

void AInjectorHolder::SetActiveLocationForInjectors()
{
	if(ActiveLocationComponent)
	{
		ActiveLocationForInjectors = ActiveLocationComponent->GetComponentLocation();
	}
}

void AInjectorHolder::SetNewActiveInjectorOnUseItem(APickupActor* PickupActor, bool IsUseItemSuccess)
{
	if(!IsUseItemSuccess || !GetWorld())
	{
		return;
	}

	PickupActor->DropItem();
	GetWorld()->DestroyActor(PickupActor);
	//CurrentInjectors.Remove(PickupActor);
	
	SetNewActiveInjector();
}

void AInjectorHolder::SetNewActiveInjector()
{
	if(CurrentInjectors.IsEmpty())
	{
		return;
	}
	
	int32 RandNum = FMath::RandRange(0, CurrentInjectors.Num() - 1);
	CurrentActiveInjector = CurrentInjectors[RandNum];
	CurrentActiveInjector->EnablePickup();
	CurrentActiveInjector->SetActorLocation(ActiveLocationForInjectors);
	CurrentActiveInjector->SetReturnOnDefaultLocation(true);
	CurrentActiveInjector->SetDefaultLocation(ActiveLocationForInjectors);
	
	CurrentInjectors.Remove(CurrentActiveInjector);
}

void AInjectorHolder::ResetInjectors()
{
	if(!GetWorld())
	{
		return;
	}

	if(IsValid(CurrentActiveInjector))
	{
		GetWorld()->DestroyActor(CurrentActiveInjector);
	}
	
	for(int i = 0; i < CurrentInjectors.Num(); i++)
	{
		APickupActor* ActorToDestroy = CurrentInjectors[i];
		ActorToDestroy->DropItem();
		GetWorld()->DestroyActor(ActorToDestroy);
	}
	
	CurrentInjectors.Reset();
}


