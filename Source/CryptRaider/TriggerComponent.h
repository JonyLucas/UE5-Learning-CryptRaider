// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mover.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "TriggerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	AActor* GetOverlappingActors() const;
	void AttachGrabbedItem();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* MoverToSet);

	UFUNCTION(BlueprintCallable)
	void ReverseMover();

private:
	UPROPERTY(EditAnywhere)
	FName OverlapTag = "Unlock";

	UPROPERTY(EditAnywhere)
	bool ShouldDisablePhysics = true;
	
	UMover* Mover = nullptr;
	AActor* AttachedActor = nullptr;
		
};
