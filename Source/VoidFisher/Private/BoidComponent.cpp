// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidComponent.h"
#include "Math/UnrealMathUtility.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"

UBoidComponent::UBoidComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	BoidDirection = FVector(FMath::RandRange(0, 1), FMath::RandRange(0, 1), FMath::RandRange(0, 1));
}

// Called when the game starts
void UBoidComponent::BeginPlay()
{
	Super::BeginPlay();
	RootComp = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	if (RootComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Root Component of %s is not a primitive"), *GetOwner()->GetName())
	}
	TempFindBoidActor();
	// ...
	
}

// Called every frame
void UBoidComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("TIIIIICK")));
	UpdateBoidBehavior(DeltaTime);
	// ...
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), SeparationRadius, 12, FColor::Red, 0, -1.f, 0, 1.f);
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), DetectNeighborRadius, 12, FColor::Green, 0, -1.f, 0, 1.f);
}

void UBoidComponent::UpdateBoidBehavior(float _DeltaTime)
{
	GetSteering(NeighboringBoids);
	SetNewRotation(_DeltaTime);
	SetNewLocation(_DeltaTime);
	/*
	//FRotator _targetRotation = FRotationMatrix::MakeFromX((GetOwner()->GetActorLocation() + _combinedForce) - GetOwner()->GetActorLocation()).Rotator();
	FRotator _targetRotation = FRotationMatrix::MakeFromX(CombinedForce).Rotator();
	FRotator _currentRotation = GetOwner()->GetActorRotation();
	FRotator _newRotation = FMath::RInterpTo(_currentRotation, _targetRotation, _DeltaTime, RotationRate);
	FVector _forwardMovement = GetOwner()->GetActorForwardVector() * MaxSpeed * _DeltaTime;
	FVector _newLocation = GetOwner()->GetActorLocation() + _forwardMovement;
	GetOwner()->SetActorRotation(_newRotation);
	GetOwner()->SetActorLocation(_newLocation);
	*/
	//RootComp->SetPhysicsLinearVelocity(_targetVelocity);
	/*
	FVector NormalizeDirection = BoidDirection.GetSafeNormal();
	FVector _Velocity = NormalizeDirection * MaxSpeed;
	if (UPrimitiveComponent* _RootComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent()))
	{
		_RootComponent->SetPhysicsLinearVelocity(_Velocity);
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Current Velocity = %s"), *_Velocity.ToString()));
	*/
}

/// <summary>
/// Retourne la direction vers la position du centre des boids
/// </summary>
/// <returns></returns>
void UBoidComponent::Cohesion(FVector& _steering, uint8& _Total, APawn& _Neighbor)
{
	float _dist = FVector::Dist(_Neighbor.GetActorLocation(), GetOwner()->GetActorLocation());
	if (_dist <= DetectNeighborRadius)
	{
		_steering += _Neighbor.GetActorLocation();
		_Total++;
	}
}

void UBoidComponent::Alignment(FVector& _steering, uint8& _Total, APawn& _Neighbor)
{
	float _dist = FVector::Dist(_Neighbor.GetActorLocation(), GetOwner()->GetActorLocation());
	if (_dist <= DetectNeighborRadius)
	{
		_steering += _Neighbor.GetActorForwardVector();
		_Total++;
	}
}

void UBoidComponent::Separation(FVector& _steering, uint8& _Total, APawn& _Neighbor)
{
	float _dist = FVector::Dist(_Neighbor.GetActorLocation(), GetOwner()->GetActorLocation());
	if (_dist <= SeparationRadius)
	{
		FVector _diff = GetOwner()->GetActorLocation() - _Neighbor.GetActorLocation();
		_diff /= _dist;
		_steering += _diff;
		_Total++;
	}
}

FVector UBoidComponent::GetCohesionDirection(FVector& _steering, uint8& _Total)
{
	_steering = _Total > 0 ? _steering / _Total : GetOwner()->GetActorForwardVector();
	_steering -= GetOwner()->GetActorLocation();
	return _steering.GetSafeNormal();
}

FVector UBoidComponent::GetAlignmentDirection(FVector& _steering, uint8& _Total)
{
	_steering = _Total > 0 ? _steering / _Total : GetOwner()->GetActorForwardVector();
	_steering -= GetOwner()->GetActorForwardVector();
	return _steering;
}

FVector UBoidComponent::GetSeparationDirection(FVector& _steering, uint8& _Total)
{
	_steering = _Total > 0 ? _steering / _Total : GetOwner()->GetActorForwardVector();
	return _steering.GetSafeNormal();
}

void UBoidComponent::SetCombinedForce(FVector& _Cohesion, FVector& _Alignment, FVector& _Separation)
{
	CombinedForce = _Cohesion + _Alignment + _Separation;
	CombinedForce = CombinedForce.GetSafeNormal();
}

void UBoidComponent::SetNewRotation(float _DeltaTime)
{
	FRotator _targetRotation = FRotationMatrix::MakeFromX(CombinedForce).Rotator();
	FRotator _currentRotation = GetOwner()->GetActorRotation();
	FRotator _newRotation = FMath::RInterpTo(_currentRotation, _targetRotation, _DeltaTime, RotationRate);

	GetOwner()->SetActorRotation(_newRotation);
}

void UBoidComponent::SetNewLocation(float _DeltaTime)
{
	FVector _forwardMovement = GetOwner()->GetActorForwardVector() * MaxSpeed * _DeltaTime;
	FVector _newLocation = GetOwner()->GetActorLocation() + _forwardMovement;

	GetOwner()->SetActorLocation(_newLocation);
}
/// <summary>
/// Calcul la cohésion, l'alignement et la séparation afin d'ajuster la force combinée des trois
/// </summary>
/// <param name="_NearestPawn"> Le tableau de pawn possédant ce component sur lequel on va looper</param>
void UBoidComponent::GetSteering(TArray<APawn*>& _NearestPawn)
{
	FVector _SteeringCohesion = FVector::ZeroVector;
	FVector _SteeringAlignment = FVector::ZeroVector;
	FVector _SteeringSeparation = FVector::ZeroVector;

	uint8 _TotalCohesion = 0;
	uint8 _TotalAlignment = 0;
	uint8 _TotalSeparation = 0;

	for (APawn* Pawn : _NearestPawn)
	{
		if (Pawn != GetOwner())
		{
			Cohesion(_SteeringCohesion, _TotalCohesion, *Pawn);
			Alignment(_SteeringAlignment, _TotalAlignment, *Pawn);
			Separation(_SteeringSeparation, _TotalSeparation, *Pawn);
		}
	}
	_SteeringCohesion = GetCohesionDirection(_SteeringCohesion, _TotalCohesion) * CohesionWeight;
	_SteeringAlignment = GetAlignmentDirection(_SteeringAlignment, _TotalAlignment) * AlignmentWeight;
	_SteeringSeparation = GetSeparationDirection(_SteeringSeparation, _TotalSeparation) * SeparationWeight;

	SetCombinedForce(_SteeringCohesion, _SteeringAlignment, _SteeringSeparation);
}

/// <summary>
/// Recupere les pawn du monde ayant boid comme component 
/// </summary>
void UBoidComponent::TempFindBoidActor()
{
	UWorld* _World = GetWorld();
	if (!_World) return;
	TArray<APawn*> PawnWithBoid;
	for (TActorIterator<APawn> It(_World); It; ++It)
	{
		APawn* _Pawn = *It;
		if (UBoidComponent* _BoidComp = _Pawn->FindComponentByClass<UBoidComponent>())
		{
			PawnWithBoid.Add(_Pawn);
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Pawn Added = %s"), *_Pawn->GetName()));
		}
	}
	NeighboringBoids = PawnWithBoid;
}

