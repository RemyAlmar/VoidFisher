// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatcheableData.h"
#include "UObject/Interface.h"
#include "Catcheable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCatcheable : public UInterface
{
	GENERATED_BODY()
};

class VOIDFISHER_API ICatcheable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Struct|GetData")
	FCatcheableData GetData();
};
