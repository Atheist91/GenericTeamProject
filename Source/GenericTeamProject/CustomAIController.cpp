// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericTeamProject.h"
#include "CustomAIController.h"
#include "CustomTypes.h"
#include "CustomPawn.h"

void ACustomAIController::BeginPlay()
{
	Super::BeginPlay();

	// Showing attitude of each pawn towards each other.
	// Just for testing purposes.
	for (TActorIterator<ACustomPawn> PawnIt(GetWorld()); PawnIt; ++PawnIt)
	{
		AController* OtherController = PawnIt->GetController();
		if (OtherController != this)
		{
			ShowAttitude(GetTeamAttitudeTowards(*OtherController), *PawnIt);
		}
	}
}

void ACustomAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	// Getting TeamID of controlled pawn.
	FTeamConfig* MyTeamConfig;
	if (UCustomTypes::GetTeamOfPawn(InPawn, MyTeamConfig))
	{
		SetGenericTeamId(MyTeamConfig->TeamID);
	}
}

ETeamAttitude::Type ACustomAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	if (OtherTeamAgent)
	{
		// Finding 'My' TeamConfig.
		FTeamConfig* MyTeamConfig;
		FName MyTeamName;
		const bool bFoundMyTeam = UCustomTypes::GetTeamByTeamID(GetGenericTeamId(), MyTeamConfig, MyTeamName);

		// Finding TeamConfig of OtherTeamAgent.
		FTeamConfig* OtherTeamConfig;
		FName OtherTeamName;
		const bool bFoundOtherTeam = UCustomTypes::GetTeamByTeamID(OtherTeamAgent->GetGenericTeamId(), OtherTeamConfig, OtherTeamName);

		// If we managed to find both TeamConfigs.
		if (bFoundMyTeam && bFoundOtherTeam)
		{
			// We're trying to get attitude towards given OtherTeamAgent
			ETeamAttitude::Type Result;
			if (MyTeamConfig->GetAttitudeTowards(OtherTeamName, Result))
			{
				return Result;
			}
			else
			{
				// If we reached this place, it means the 'My' Team has no specified relations towards the OtherTeamAgent's Team.
				UE_LOG(Code, Warning, TEXT("Couldn't find what's the attitude %s (Team: %s) towards [%s] (Team: %s). It means that the desired relation wasn't defined in DataTable with Teams."), *GetName(), *MyTeamName.ToString(), *Other.GetName(), *OtherTeamName.ToString());
			}
		}
		else
		{
			// error
		}
	}

	return ETeamAttitude::Neutral;
}