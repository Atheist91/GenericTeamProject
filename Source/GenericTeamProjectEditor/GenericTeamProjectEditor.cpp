// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GenericTeamProjectEditor.h"
#include "CustomTypes.h"
#include "TeamPinFactory.h"

class FGenericTeamProjectEditor : public IModuleInterface
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
		//create your factory and shared pointer to it.
		TSharedPtr<FTeamPinFactory> TeamPinFactory = MakeShareable(new FTeamPinFactory());
		//and now register it.
		FEdGraphUtilities::RegisterVisualPinFactory(TeamPinFactory);
	}
	virtual void ShutdownModule() override
	{

	}
};

IMPLEMENT_MODULE(FGenericTeamProjectEditor, GenericTeamProjectEditor)