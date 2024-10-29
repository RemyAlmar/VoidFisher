// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatcheableData.generated.h"

USTRUCT(BlueprintType)
struct VOIDFISHER_API FCatcheableData
{
public:
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Size = 1.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name = "DefaultName";


	FCatcheableData();
	~FCatcheableData();
};
