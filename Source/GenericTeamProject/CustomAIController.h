// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CustomTypes.h"
#include "CustomAIController.generated.h"

/**
 * 
 */
UCLASS()
class GENERICTEAMPROJECT_API ACustomAIController : public AAIController
{
	GENERATED_BODY()
	
public:

protected:

private:

public:

	virtual void Tick(float DeltaTime) override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	// Assigns this pawn to new Team.
	// @param InTeam - The Team to which this Pawn will be assigned.
	// @returns - Whether Pawn was successfully assigned to a Team or not.
	UFUNCTION(BlueprintCallable, Category = "AI Team")
	virtual bool SetTeam(FTeam InTeam);

protected:

	//
	UFUNCTION(BlueprintImplementableEvent, Category = "AI Team Preview")
	void ShowAttitude(ETeamAttitude::Type InAttitude, ACustomPawn* InPawn);

private:
	
};