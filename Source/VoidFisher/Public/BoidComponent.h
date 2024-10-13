// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoidComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Category="Boid")
class VOIDFISHER_API UBoidComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBoidComponent();

	TArray<APawn*> NeighboringBoids;

	void UpdateBoidBehavior(float _DeltaTime);

private:
	FVector BoidDirection = FVector::ZeroVector;
	UPrimitiveComponent* RootComp = nullptr;

	FVector Cohesion();
	FVector Alignment();
	void Separation();

	void TempFindBoidActor();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidParameters", meta = (ClampMin = "0", ClampMax = "500"));
	float MaxSpeed = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidParameters", meta = (ClampMin = "0", ClampMax = "500"));
	float RotationRate = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidParameters", meta = (ClampMin = "0", ClampMax = "5000"));
	float DetectNeighborRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidParameters", meta = (ClampMin = "5", ClampMax = "200"));
	float SeparationRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidParameters", meta = (ClampMin = "0", ClampMax = "1"));
	float CohesionWeight = .5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidParameters", meta = (ClampMin = "0", ClampMax = "1"));
	float AlignmentWeight = .5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidParameters", meta = (ClampMin = "0", ClampMax = "1"));
	float SeparationWeight = .5f;
};
