// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericTeamProject.h"
#include "CustomAIController.h"
#include "CustomTypes.h"
#include "CustomPawn.h"

void ACustomAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

ETeamAttitude::Type ACustomAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	if (OtherTeamAgent)
	{
		// Finding 'My' TeamConfig.
		FTeamConfig* MyTeamConfig;
		FName MyTeamName;
		const bool bFoundMyTeam = UCustomTypes::GetTeamByID(GetGenericTeamId(), MyTeamConfig, MyTeamName);

		// Finding TeamConfig of OtherTeamAgent.
		FTeamConfig* OtherTeamConfig;
		FName OtherTeamName;
		const bool bFoundOtherTeam = UCustomTypes::GetTeamByID(OtherTeamAgent->GetGenericTeamId(), OtherTeamConfig, OtherTeamName);

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
				UE_LOG(Code, Warning, TEXT("Couldn't find what's the attitude %s (Team: %s) towards [%s] (Team: %s). It means that the desired relation wasn't defined in DataTable with Teams. Returning Neutral attitude."), *GetName(), *MyTeamName.ToString(), *Other.GetName(), *OtherTeamName.ToString());
			}
		}
		else
		{
			if (!bFoundMyTeam) UE_LOG(Code, Error, TEXT("Couldn't find mine [%s] TeamConfig. See the error above. Returning Neutral attitude."), *GetName());
			if (!bFoundOtherTeam) UE_LOG(Code, Error, TEXT("Couldn't find TeamConfig of given Actor [%s]. See the error above. Returning Neutral attitude."), *Other.GetName());
		}
	}
	else
	{
		UE_LOG(Code, Warning, TEXT("Given Actor [%s] doesn't implement IGenericTeamAgentInterface interface. Returning Neutral attitude."), *Other.GetName());
	}

	return ETeamAttitude::Neutral;
}

bool ACustomAIController::SetTeam(FTeam InTeam)
{
	FTeamConfig* NewTeamConfig = UCustomTypes::GetTeamByName(InTeam.Name);
	if (NewTeamConfig)
	{
		SetGenericTeamId(NewTeamConfig->TeamID);
		return true;
	}
	else
	{		
		UE_LOG(Code, Error, TEXT("Can't assign AIController [%s] to team [%s] because the config for desired Team couldn't be found. See errors/warnings above."), *GetName(), *InTeam.Name.ToString());
	}

	return false;
}