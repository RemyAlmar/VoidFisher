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
	FVector CombinedForce = FVector::ZeroVector;
	UPrimitiveComponent* RootComp = nullptr;

	void GetSteering(TArray<APawn*>& _NearestPawn);

	void Cohesion(FVector& _steering, uint8& _Total, APawn& _Neighbor);
	void Alignment(FVector& _steering, uint8& _Total, APawn& _Neighbor);
	void Separation(FVector& _steering, uint8& _Total, APawn& _Neighbor);

	FVector GetCohesionDirection(FVector& _steering, uint8& _Total);
	FVector GetAlignmentDirection(FVector& _steering, uint8& _Total);
	FVector GetSeparationDirection(FVector& _steering, uint8& _Total);

	void SetCombinedForce(FVector& _Cohesion, FVector& _Alignment, FVector& _Separation);
	void SetNewRotation(float _DeltaTime);
	void SetNewLocation(float _DeltaTime);


	void TempFindBoidActor();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidStats", meta = (ClampMin = "0", ClampMax = "500"));
	float MaxSpeed = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidStats", meta = (ClampMin = "0", ClampMax = "500"));
	float RotationRate = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidAreaDetection", meta = (ClampMin = "0", ClampMax = "5000"));
	float DetectNeighborRadius = 2500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidAreaDetection", meta = (ClampMin = "5", ClampMax = "2500"));
	float SeparationRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidWeightForce", meta = (ClampMin = "0", ClampMax = "1"));
	float CohesionWeight = .3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidWeightForce", meta = (ClampMin = "0", ClampMax = "1"));
	float AlignmentWeight = .5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidWeightForce", meta = (ClampMin = "0", ClampMax = "1"));
	float SeparationWeight = .8f;
};
