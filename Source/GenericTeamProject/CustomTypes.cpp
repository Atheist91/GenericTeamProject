// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericTeamProject.h"
#include "CustomTypes.h"
#include "CustomPawn.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"

FTeam::FTeam()
{
	Name = FName("");
	RowHandler.DataTable = UCustomTypes::GetTeamsDataTable();
}

FTeamConfig* FTeam::GetTeamConfig() const
{
	return RowHandler.GetRow<FTeamConfig>();
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

bool UCustomTypes::GetTeamByTeamID(FGenericTeamId InTeamID, FTeamConfig*& OutTeamConfig, FName& OutTeamName)
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

bool UCustomTypes::GetTeamOfPawn(APawn* InPawn, FTeamConfig*& OutTeamConfig)
{
	ACustomPawn* CustomPawn = Cast<ACustomPawn>(InPawn);
	FTeamConfig* Result = CustomPawn ? CustomPawn->GetTeamConfig() : nullptr;
	if (Result)
	{
		OutTeamConfig = Result;
		return true;
	}
	else
	{
		if (!InPawn) UE_LOG(Code, Warning, TEXT("Can't get TeamConfig of because given Pawn is null."));
		if (!CustomPawn && InPawn) UE_LOG(Code, Warning, TEXT("Can't get TeamConfig of given Pawn [%s] because the Pawn doesn't derive from CustomPawn class."), *InPawn->GetName());
		if (CustomPawn && !Result) UE_LOG(Code, Warning, TEXT("Can't get TeamConfig of given Pawn [%s]. Probably the team of that Pawn isn't configured properly."), *CustomPawn->GetName());
	}
	
	return false;
}

void UCustomTypes::GetTeamName(FTeam InTeam, FName& OutTeamName, FName& OutTeamName2)
{
	OutTeamName = InTeam.Name;
	OutTeamName2 = InTeam.RowHandler.RowName;
}

UDataTable* UCustomTypes::GetTeamsDataTable()
{
	return TeamsDataTable;
}