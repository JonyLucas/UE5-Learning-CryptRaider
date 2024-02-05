// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


AActor* UTriggerComponent::GetOverlappingActors() const
{
	TArray<AActor*> OverlappingActors;
	UPrimitiveComponent::GetOverlappingActors(OverlappingActors);

	// if(OverlappingActors.Num() > 0)
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("Overlapping Actor: %s"), *OverlappingActors[0]->GetName());
	// }

	// for(int i = 0; i < OverlappingActors.Num(); i++)
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("Overlapping Actor: %s"), *OverlappingActors[i]->GetName());
	// }

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->ActorHasTag(OverlapTag))
		{
			// UE_LOG(LogTemp, Display, TEXT("Overlapping Actor: %s"), *Actor->GetName());
			return Actor;
		}
	}

	return nullptr;
}

void UTriggerComponent::AttachGrabbedItem()
{
	AActor* OverlapActor = GetOverlappingActors();

	if (!Mover)
	{
		return;
	}

	if (OverlapActor && !AttachedActor)
	{
		if (!OverlapActor->ActorHasTag("Grabbed") && !OverlapActor->IsAttachedTo(GetOwner()))
		{
			AttachedActor = OverlapActor;
			UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OverlapActor->GetRootComponent());
			if (PrimitiveComponent && ShouldDisablePhysics)
			{
				PrimitiveComponent->SetSimulatePhysics(false);
			}
			OverlapActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
			Mover->SetShouldMove(true, false);
		}
	}

	if(AttachedActor && AttachedActor->ActorHasTag("Grabbed"))
	{
		AttachedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AttachedActor = nullptr;
		ReverseMover();
	}
}

// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AttachGrabbedItem();
}

void UTriggerComponent::SetMover(UMover* MoverToSet)
{
	Mover = MoverToSet;
}

void UTriggerComponent::ReverseMover()
{
	UE_LOG(LogTemp, Display, TEXT("ReverseMover"));
	Mover->SetShouldMove(true, true);
}
