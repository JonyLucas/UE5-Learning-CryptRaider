// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	// Usage of References
	float FloatVal = 0;
	float& FloatRef = FloatVal;
	FloatRef += 10;
	UE_LOG(LogTemp, Display, TEXT("FloatVal is %f"), FloatVal);

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("PhysicsHandle is %s"), *PhysicsHandle->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle is null"));
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		const FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}

	// FRotator Rotation = GetComponentRotation();
	// UE_LOG(LogTemp, Display, TEXT("Rotation is %s"), *Rotation.ToCompactString());
	//
	// float Time = GetWorld()->GetTimeSeconds();
	// UE_LOG(LogTemp, Display, TEXT("Time is %f"), Time);
}

void UGrabber::DebugTrace(const FHitResult& HitResult) const
{
	const FVector HitLocation = HitResult.Location;
	const FVector HitPoint = HitResult.ImpactPoint;
	DrawDebugSphere(GetWorld(), HitLocation, 10, 12, FColor::Green, false, 5); // The Hit Location of the sweep shape
	DrawDebugSphere(GetWorld(), HitPoint, 10, 12, FColor::Yellow, false, 5); // Location of the Hit
	UE_LOG(LogTemp, Display, TEXT("Hit %s"), *HitResult.GetActor()->GetActorNameOrLabel());
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Red,
	                                 FString::Printf(TEXT("Hit %s"), *HitResult.GetActor()->GetActorNameOrLabel()));
}

bool UGrabber::GetTraceResult(FHitResult& HitResult) const
{
	const FVector StartPoint = GetComponentLocation();
	const FVector EndPoint = StartPoint + GetForwardVector() * GrabDistance;
	// DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red);
	// DrawDebugSphere(GetWorld(), EndPoint, 10, 12, FColor::Blue, false, 5); // Location of the EndPoint

	return GetWorld()->SweepSingleByChannel(
		HitResult,
		StartPoint, EndPoint,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(GrabRadius));
}

void UGrabber::Grab()
{
	if (!PhysicsHandle || PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		return;
	}

	FHitResult HitResult;
	if (GetTraceResult(HitResult))
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.Location,
			GetComponentRotation()
		);

		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HitResult.GetComponent()->SetSimulatePhysics(true);
		
		// DebugTrace(HitResult);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Actor Hitted"));
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released"));
	if(PhysicsHandle)
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
}
