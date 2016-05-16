// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/AIModule/Classes/GenericTeamAgentInterface.h"
#include "CustomTypes.generated.h"

// Made just to be able to create custom pin for specific type in editor.
// Probably it would be possible to remove it and create custom pin for FTeamConfig instead.
USTRUCT(BlueprintType)
struct FTeam
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FName Name;

	FTeam();
};

// Struct used to determine attitude of a Team towards OtherTeam.
// NOTE: The relation isn't bidirectional, so if Team A is 
// supposed to be hostile towards Team B and the other way around
// Team A has to have TeamAttitudeConfig set up as 'hostile towards Team B' 
// and Team B has to have TeamAttitudeConfig set up as 'hostile towards Team A'.
USTRUCT(BlueprintType)
struct FTeamAttitudeConfig
{
	GENERATED_USTRUCT_BODY()

public:

	// Name of the other team towards which we're setting up the attitude.
	// NOTE: It has to be a Row Name from DataTable containing Teams configurations.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName OtherTeam;

	// The attitude of this Team towards OtherTeam.
	// NOTE: The relation isn't bidirectional, so if Team A is 
	// supposed to be hostile towards Team B and the other way around
	// Team A has to have TeamAttitudeConfig set up as 'hostile towards Team B' 
	// and Team B has to have TeamAttitudeConfig set up as 'hostile towards Team A'.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETeamAttitude::Type> Attitude;

	FTeamAttitudeConfig();

};

// Holds whole configuration of a single AI Team.
// Contains stuff such as TeamID (used by Generic Team system in UE4), DisplayName, Attitudes towards other Teams.
USTRUCT(BlueprintType)
struct FTeamConfig : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	// The TeamID that is used in Generic AI Team system in UE4.
	// NOTE: Has to be unique per team.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGenericTeamId TeamID;

	// Display name of given Team.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	// List of attitudes towards other Teams.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTeamAttitudeConfig> Attitudes;

	FTeamConfig();

	// Gets attitude of this Team towards InTeamName.
	// @param InTeamName: Name of a Team towards which we want to know the attitude. (Must be one of Row Names in DataTable)
	// @param OutAttitude: Attitude towards given Team.
	// @returns: Whether we successfully took the attitude or not.
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

	// Searches for given Team in TeamsDataTable and then returns the row (if exists).
	// @param InTeamName: Name of a Team for which the TeamCOnfig will be returned. (Has to be an exact RowName).
	// @returns: TeamConfig of desired Team.
	static FTeamConfig* GetTeamByName(FName InTeamName);

	// Gets info about a Team basing on TeamID.
	// @param InTeamID - ID of a Team of which we want to get info.
	// @param OutTeamConfig - Config of desired Team.
	// @param OutTeamName - Name of desired Team.
	// @returns - Whether we were able to find the Team using given InTeamID or not.
	static bool GetTeamByID(FGenericTeamId InTeamID, FTeamConfig*& OutTeamConfig, FName& OutTeamName);

	// Gets info about a Team to which given Pawn belongs.
	// @param InPawn - Pawn whos Team we want to find.
	// @param OutTeamConfig - Config of a team to which given InPawn belongs.
	// @param OutTeamName - Name of desired Team.
	// @returns - Whether we were able to get TeamConfig from InPawn or not.
	static bool GetTeamOfPawn(APawn* InPawn, FTeamConfig*& OutTeamConfig, FName& OutTeamName);

	// Gets DataTable containing info about Teams and their relations between each other.
	static UDataTable* GetTeamsDataTable();

	//
	UFUNCTION(BlueprintPure, Category = "AI Team")
	static bool GetTeamName(APawn* InPawn, FName& OutName);
	
};