// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericTeamProject.h"
#include "CustomPawn.h"
#include "AIController.h"

ACustomPawn::ACustomPawn()
{
	Team = FTeam();
}

void ACustomPawn::SetTeam(FTeam InTeam)
{
	AAIController* MyController = Cast<AAIController>(GetController());
	FTeamConfig* NewTeamConfig = InTeam.GetTeamConfig();
	if (NewTeamConfig && MyController)
	{
		Team = InTeam;

		// Setting new TeamID on controlling controller.
		MyController->SetGenericTeamId(NewTeamConfig->TeamID);
	}
	else
	{
		if (!MyController) UE_LOG(Code, Error, TEXT("Can't assign pawn %s to team [%s|%s] because the pawn isn't controlled by AIController."), *GetName(), *InTeam.Name.ToString(), *InTeam.RowHandler.RowName.ToString());
		if (!NewTeamConfig && InTeam.RowHandler.DataTable) UE_LOG(Code, Error, TEXT("Can't assign pawn %s to team [%s|%s] because selected DataTable doesn't such Team."), *GetName(), *InTeam.Name.ToString(), *InTeam.RowHandler.RowName.ToString());
		if (!NewTeamConfig && !InTeam.RowHandler.DataTable) UE_LOG(Code, Error, TEXT("Can't assign pawn %s to team [%s|%s] because DataTable of given Team is null."), *GetName(), *InTeam.Name.ToString(), *InTeam.RowHandler.RowName.ToString());
	}
}

FTeamConfig* ACustomPawn::GetTeamConfig() const
{
	return Team.RowHandler.GetRow<FTeamConfig>();
}