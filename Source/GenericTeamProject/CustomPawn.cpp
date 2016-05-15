// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericTeamProject.h"
#include "CustomPawn.h"
#include "AIController.h"

bool ACustomPawn::SetTeam(FTeam InTeam)
{
	AAIController* MyController = Cast<AAIController>(GetController());
	FTeamConfig* NewTeamConfig = UCustomTypes::GetTeamConfig(InTeam);
	if (NewTeamConfig && MyController)
	{
		// Setting new TeamID on controlling controller.
		MyController->SetGenericTeamId(NewTeamConfig->TeamID);
		return true;
	}
	else
	{
		if (!MyController) UE_LOG(Code, Error, TEXT("Can't assign pawn [%s] to team [%s] because the pawn isn't controlled by AIController."), *GetName(), *InTeam.Name.ToString());
		if (!NewTeamConfig) UE_LOG(Code, Error, TEXT("Can't assign pawn [%s] to team [%s] because the config for desired team couldn't be found. It probably means that the Team doesn't exist in DataTable containing Teams."), *GetName(), *InTeam.Name.ToString());		
	}

	return false;
}