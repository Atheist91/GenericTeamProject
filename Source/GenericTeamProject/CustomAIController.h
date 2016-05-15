// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
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

	virtual void BeginPlay() override;

	virtual void Possess(APawn* InPawn) override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "AI Team Preview")
	void ShowAttitude(ETeamAttitude::Type InAttitude, ACustomPawn* InPawn);

private:
	
};