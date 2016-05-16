// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "CustomTypes.h"
#include "CustomPawn.generated.h"

UCLASS()
class GENERICTEAMPROJECT_API ACustomPawn : public APawn
{
	GENERATED_BODY()

public:

protected:

private:

public:

	// Assigns this pawn to new Team.
	// @param InTeam - The Team to which this Pawn will be assigned.
	// @returns - Whether Pawn was successfully assigned to a Team or not.
	UFUNCTION(BlueprintCallable, Category = "AI Team")
	virtual bool SetTeam(FTeam InTeam);

protected:

private:
	
};