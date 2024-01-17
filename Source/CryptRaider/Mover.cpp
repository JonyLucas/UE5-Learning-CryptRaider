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

	OriginalLocation = Pointer->GetActorLocation();
	TargetLocation = OriginalLocation + MoveOffset;
	Speed = MoveOffset.Size() / MoveDuration;

	UE_LOG(LogTemp, Display, TEXT("Owner is %s: with address %p and position %s"), *Pointer->GetActorNameOrLabel(),
	       Pointer, *OriginalLocation.ToCompactString());
}


void UMover::Move(float DeltaTime)
{
	AActor* Owner = GetOwner();
	FVector CurrentLocation = Owner->GetActorLocation();
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
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
