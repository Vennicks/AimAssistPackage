// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_Game.generated.h"

/**
 * 
 */
UCLASS()
class AIMASSISTPACKAGE_API AGM_Game : public AGameMode
{
	GENERATED_BODY()
public:
	void HandleDeathPlayer();
};
