// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreatureType.generated.h"

UENUM(BlueprintType)
enum class ECreatureType : uint8
{
	Creature_01 UMETA(DisplayName = "Creature_01"),
	Creature_02 UMETA(DisplayName = "Creature_02"),
	Creature_03 UMETA(DisplayName = "Creature_03"),
	Creature_04 UMETA(DisplayName = "Creature_04")
};
