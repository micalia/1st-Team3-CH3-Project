// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "EGunType.generated.h"

UENUM(Blueprintable)

enum class EGunType : uint8
{
	Rifle UMETA(DisplayName = "Rifle"),
	Pistol UMETA(DisplayName = "Pistol")
};
