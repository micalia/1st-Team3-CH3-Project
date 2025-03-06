// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "EnumsDefinitions.generated.h"

//class THREEFPS_API EnumsDefinitions
//{
//public:
//	EnumsDefinitions();
//	~EnumsDefinitions();
//};

UENUM(BlueprintType)
enum class EPATROLTYPE :uint8
{
	TargetKey  UMETA(DisplayName = "TargetKey"),
	Empty      UMETA(DisplayName = "Empty"),
	Random     UMETA(DisplayName = "Random"),
	Chase      UMETA(DisplayName = "Chase")
};

UENUM(BlueprintType)
enum class ESNDZOMBIE :uint8
{
	ATT1,
	ATT2,
	DAMAGE,
	DIE
};