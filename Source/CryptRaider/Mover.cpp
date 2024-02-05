// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Mover reporting for duty!"));
	AActor* Pointer = GetOwner();

	InitialActorLocation = Pointer->GetActorLocation();
	OriginalTargetLocation = InitialActorLocation + MoveOffset;
	CurrentTargetLocation = OriginalTargetLocation;
	Speed = MoveOffset.Size() / MoveDuration;

	UE_LOG(LogTemp, Display, TEXT("Owner is %s: with address %p and position %s"), *Pointer->GetActorNameOrLabel(),
	       Pointer, *InitialActorLocation.ToCompactString());
}


void UMover::Move(float DeltaTime)
{
	AActor* Owner = GetOwner();
	const FVector CurrentLocation = Owner->GetActorLocation();

	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, CurrentTargetLocation, DeltaTime, Speed);
	if ((NewLocation - CurrentTargetLocation).Size() < 0.1f)
	{
		NewLocation = CurrentTargetLocation;
		ShouldMove = false;
	}

	Owner->SetActorLocation(NewLocation);
}

// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (ShouldMove)
	{
		Move(DeltaTime);
	}
}

void UMover::SetShouldMove(bool ShouldMoveToSet, bool IsReverse)
{
	ShouldMove = ShouldMoveToSet;
	CurrentTargetLocation = IsReverse ? InitialActorLocation : OriginalTargetLocation;
}

bool UMover::IsReversed() const
{
	return CurrentTargetLocation == InitialActorLocation;
}
