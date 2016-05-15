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

	// A team to which this Pawn will belong.
	// Team.DataTable has to contain a reference to DataTable asset containing teams and their relations.  Then, the dropdown of available teams will show up.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	FTeam Team;

private:

public:

	ACustomPawn();

	// Moves this pawn to new team.	
	UFUNCTION(BlueprintCallable, Category = "AI Team")
	virtual void SetTeam(FTeam InTeam);

	// Returns config of a Team to which this Pawn belongs.
	FTeamConfig* GetTeamConfig() const;

protected:

private:
	
};