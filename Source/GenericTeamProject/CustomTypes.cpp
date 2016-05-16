// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericTeamProject.h"
#include "CustomTypes.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "AIController.h"

FTeam::FTeam()
{
	Name = FName("");
}

FTeamAttitudeConfig::FTeamAttitudeConfig()
{
	OtherTeam = FName("None");
	Attitude = ETeamAttitude::Neutral;
}

FTeamConfig::FTeamConfig()
{

}

bool FTeamConfig::GetAttitudeTowards(FName InTeamName, ETeamAttitude::Type& OutAttitude) const
{
	for (FTeamAttitudeConfig ItAttitude : Attitudes)
	{
		if (ItAttitude.OtherTeam == InTeamName)
		{
			OutAttitude = ItAttitude.Attitude;
			return true;
		}
	}

	return false;
}

UDataTable* UCustomTypes::TeamsDataTable;

UCustomTypes::UCustomTypes()
{
	// Getting reference to proper data table asset
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("DataTable'/Game/DT_Teams.DT_Teams'"));
	TeamsDataTable = DataTableAsset.Object;
}

FTeamConfig* UCustomTypes::GetTeamByName(FName InTeamName)
{
	if (TeamsDataTable)
	{
		FString Context;
		FTeamConfig* Result = TeamsDataTable->FindRow<FTeamConfig>(InTeamName, Context);
		if (Result)
		{
			return Result;
		}
		else
		{
			UE_LOG(Code, Error, TEXT("There is no such [%s] Team in TeamsDataTable [%s]. Context: [%s]"), *InTeamName.ToString(), *TeamsDataTable->GetName(), *Context);
		}
	}
	else
	{
		UE_LOG(Code, Error, TEXT("Couldn't get TeamConfig for Team [%s] because TeamsDataTable is null. See Constructor of UCustomTypes and see if the path matches to actuall asset."), *InTeamName.ToString());
	}

	return nullptr;
}

bool UCustomTypes::GetTeamByID(FGenericTeamId InTeamID, FTeamConfig*& OutTeamConfig, FName& OutTeamName)
{
	if (TeamsDataTable)
	{
		// Going through every row in DataTable
		for (auto It = TeamsDataTable->RowMap.CreateConstIterator(); It; ++It)
		{
			const FName TeamName = It.Key();
			FTeamConfig* TeamConfig = GetTeamByName(TeamName);
			if (TeamConfig && TeamConfig->TeamID == InTeamID)
			{
				UE_LOG(Code, Log, TEXT("Found Team [%s] in TeamsDataTable [%s] which uses given TeamID [%d]."), *TeamName.ToString(), *TeamsDataTable->GetName(), InTeamID.GetId());

				OutTeamName = TeamName;
				OutTeamConfig = TeamConfig;
				return true;
			}
		}

		// If we reached this place, it means that DataTable (TeamsDataTable) doesn't contain a Team that uses given TeamID.
		UE_LOG(Code, Warning, TEXT("TeamsDataTable [%s] doesn't contain a team, that uses %d TeamID."), *TeamsDataTable->GetName(), InTeamID.GetId());
	}
	else
	{
		UE_LOG(Code, Error, TEXT("Couldn't get TeamConfig for TeamID [%d] because TeamsDataTable is null. See Constructor of UCustomTypes and see if the path matches to actuall asset."), InTeamID.GetId());
	}

	return false;
}

bool UCustomTypes::GetTeamOfPawn(APawn* InPawn, FTeamConfig*& OutTeamConfig, FName& OutTeamName)
{
	AAIController* Controller = InPawn ? Cast<AAIController>(InPawn->GetController()) : nullptr;
	FTeamConfig* Result = nullptr;
	FName TeamName;
	if (Controller && UCustomTypes::GetTeamByID(Controller->GetGenericTeamId(), Result, TeamName))
	{
		OutTeamConfig = Result;
		OutTeamName = TeamName;
		return true;
	}
	else
	{
		if (!InPawn) UE_LOG(Code, Warning, TEXT("Couldn't get TeamConfig because given Pawn is null."));
		if (!Controller && InPawn) UE_LOG(Code, Warning, TEXT("Couldn't get TeamConfig of given Pawn [%s] because the Pawn isn't controlled by AIController."), *InPawn->GetName());
		if (Controller && !Result) UE_LOG(Code, Warning, TEXT("Couldn't get TeamConfig of given Pawn [%s]. See errors/warnings above."), *InPawn->GetName());
	}

	return false;
}

bool UCustomTypes::GetTeamName(APawn* InPawn, FName& OutName)
{
	FTeamConfig* TeamConfig = nullptr;
	return GetTeamOfPawn(InPawn, TeamConfig, OutName);
}

UDataTable* UCustomTypes::GetTeamsDataTable()
{
	return TeamsDataTable;
}