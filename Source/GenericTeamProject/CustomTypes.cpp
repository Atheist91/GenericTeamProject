// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericTeamProject.h"
#include "CustomTypes.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "AIController.h"

FTeam::FTeam()
{
	Name = FName("None");
}

FTeamAttitudeConfig::FTeamAttitudeConfig()
{
	OtherTeam = FName("");
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

FTeamConfig* UCustomTypes::GetTeamConfig(FTeam InTeam)
{
	FTeamConfig* Result = nullptr;

	if (TeamsDataTable)
	{
		FString Context;

		Result = TeamsDataTable->FindRow<FTeamConfig>(InTeam.Name, Context);
		if (!Result)
		{
			// error, couldn't find team
		}
	}
	else
	{
		// error wrong ref to data table
	}

	return Result;
}

bool UCustomTypes::GetTeamConfigByTeamID(FGenericTeamId InTeamID, FTeamConfig*& OutTeamConfig, FName& OutTeamName)
{
	if (TeamsDataTable)
	{
		for (auto It = TeamsDataTable->RowMap.CreateConstIterator(); It; ++It)
		{
			const FName TeamName = It.Key();
			const FString UselessVar = FString("");
			FTeamConfig* TeamConfig = TeamsDataTable->FindRow<FTeamConfig>(TeamName, UselessVar);
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
		UE_LOG(Code, Error, TEXT("Can't get TeamConfig because UCustomTypes class wasn't able to get reference to DataTable containing Teams. Make sure that the path in constructor matches to the path of the actual asset."));
	}

	return false;
}

bool UCustomTypes::GetTeamOfPawn(APawn* InPawn, FTeamConfig*& OutTeamConfig, FName& OutTeamName)
{
	AAIController* Controller = InPawn ? Cast<AAIController>(InPawn->GetController()) : nullptr;
	FTeamConfig* Result = nullptr;
	FName TeamName;
	if (Controller && UCustomTypes::GetTeamConfigByTeamID(Controller->GetGenericTeamId(), Result, TeamName))
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