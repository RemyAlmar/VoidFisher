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
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), DetectNeighborRadius, 12, FColor::Red, 0, -1.f, 0, 1.f);
}

void UBoidComponent::UpdateBoidBehavior(float _DeltaTime)
{
	FVector _newVelocity = FVector::Zero();
	//FVector _alignment = Alignment();
	FVector _direction = Cohesion().GetSafeNormal();
	//_newVelocity += _alignment * _DeltaTime;
	_newVelocity += _direction * _DeltaTime * MaxSpeed;
	FVector _targetVelocity = _newVelocity;
	FRotator _targetRotation = FRotationMatrix::MakeFromX((GetOwner()->GetActorLocation() + _targetVelocity) - GetOwner()->GetActorLocation()).Rotator();
	FRotator _rotation = FMath::RInterpTo(GetOwner()->GetActorRotation(), _targetRotation, _DeltaTime, RotationRate);
	GetOwner()->SetActorRotation(_rotation);
	RootComp->SetPhysicsLinearVelocity(_targetVelocity);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Target Rotation for %s = %s"), *GetOwner()->GetName(), *_targetRotation.ToString()));
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

FVector UBoidComponent::Cohesion()
{
	FVector _steering = FVector::ZeroVector;
	uint8 _total = 0;
	for (APawn* Pawn : NeighboringBoids)
	{
		if (Pawn != GetOwner())
		{
			float _dist = FVector::Dist(Pawn->GetActorLocation(), GetOwner()->GetActorLocation());
			if (_dist <= DetectNeighborRadius)
			{
				_steering += Pawn->GetActorLocation();
				_total++;
			}
		}
	}
	_steering = _total > 0 ? _steering / _total : _steering;
	_steering -= GetOwner()->GetActorLocation();
	return _steering;
}

FVector UBoidComponent::Alignment()
{
	FVector _steering = FVector::ZeroVector;
	uint8 _total = 0;
	for (APawn* Pawn : NeighboringBoids)
	{
		if (Pawn != GetOwner())
		{
			float _dist = FVector::Dist(Pawn->GetActorLocation(), GetOwner()->GetActorLocation());
			if (_dist <= DetectNeighborRadius)
			{
				_steering += Pawn->GetActorForwardVector();
				_total++;
			}
		}
	}
	_steering = _total > 0 ? _steering /_total : _steering;
	_steering -= GetOwner()->GetActorForwardVector();
	return _steering;
}

void UBoidComponent::Separation()
{
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

