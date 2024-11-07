// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoidFisher/Enum/CreatureType.h"
#include "CatcheableData.generated.h"

USTRUCT(BlueprintType)
struct VOIDFISHER_API FCatcheableData
{
public:
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECreatureType CreatureType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Quantity = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Size = 1.2f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Weight = 1.2f;



	FCatcheableData();
	~FCatcheableData();
};
