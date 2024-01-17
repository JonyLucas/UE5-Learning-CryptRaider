// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DebugTrace(const FHitResult& HitResult) const;
	bool GetTraceResult(FHitResult& HitResult) const;

	UFUNCTION(BlueprintCallable)
	void Grab();
	
	UFUNCTION(BlueprintCallable)
	void Release();
	
private:
	UPROPERTY(EditAnywhere)
	float GrabDistance = 300;

	UPROPERTY(EditAnywhere)
	float HoldDistance = 150;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 100;

	UPhysicsHandleComponent* PhysicsHandle;
};
