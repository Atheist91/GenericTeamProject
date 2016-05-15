// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/AIModule/Classes/GenericTeamAgentInterface.h"
#include "CustomTypes.generated.h"

USTRUCT(BlueprintType)
struct FTeam
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FName Name;

	FTeam();
};

USTRUCT(BlueprintType)
struct FTeamAttitudeConfig
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName OtherTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETeamAttitude::Type> Attitude;

	FTeamAttitudeConfig();

};

USTRUCT(BlueprintType)
struct FTeamConfig : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGenericTeamId TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTeamAttitudeConfig> Attitudes;

	FTeamConfig();

	bool GetAttitudeTowards(FName InTeamName, ETeamAttitude::Type& OutAttitude) const;
};

/**
 * 
 */
UCLASS()
class GENERICTEAMPROJECT_API UCustomTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
			
protected:

	// DataTable that holds data about Teams and their relations between each other.
	static class UDataTable* TeamsDataTable;

public:

	UCustomTypes();

	static FTeamConfig* GetTeamConfig(FTeam InTeam);

	// Gets info about a Team basing on TeamID.
	// @param InTeamID - ID of a Team of which we want to get info.
	// @param OutTeamConfig - Config of desired Team.
	// @param OutTeamName - Name of desired Team.
	// @returns - Whether we were able to find the Team using given InTeamID or not.
	static bool GetTeamConfigByTeamID(FGenericTeamId InTeamID, FTeamConfig*& OutTeamConfig, FName& OutTeamName);

	// Gets info about a Team to which given Pawn belongs.
	// @param InPawn - Pawn whos Team we want to find.
	// @param OutTeamConfig - Config of a team to which given InPawn belongs.
	// @param OutTeamName - Name of desired Team.
	// @returns - Whether we were able to get TeamConfig from InPawn or not.
	static bool GetTeamOfPawn(APawn* InPawn, FTeamConfig*& OutTeamConfig, FName& OutTeamName);	

	UFUNCTION(BlueprintPure, Category = "AI Team")
	static bool GetTeamName(APawn* InPawn, FName& OutName);

	// Gets DataTable containing info about Teams and their relations between each other.
	static UDataTable* GetTeamsDataTable();
};