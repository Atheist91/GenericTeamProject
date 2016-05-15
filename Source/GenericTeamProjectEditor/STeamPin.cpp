#include "GenericTeamProjectEditor.h" 
#include "KismetEditorUtilities.h" 
#include "STextComboBox.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "SGraphPin.h"
#include "Runtime/Engine/Classes/EdGraph/EdGraph.h"

#include "STeamPin.h"
#include "CustomTypes.h"

void STeamPin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{ 
	// Getting DataTable containing info about Teams.
	UDataTable* DT = UCustomTypes::GetTeamsDataTable();
	if (DT)
	{
		// Building list of options that will be available in combo box widget.
		for (FName TeamName : DT->GetRowNames())
		{
			FString* Str = new FString(TeamName.ToString());
			TeamsList.Add(MakeShareable(Str));
		}
	}
	
	FDelegateHandle Smth = InGraphPinObj->GetOwningNode()->GetGraph()->AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateSP(this, &STeamPin::OnGraphChanged));
	UEdGraph* Graph = InGraphPinObj->GetOwningNode()->GetGraph();

	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

TSharedRef<SWidget>	STeamPin::GetDefaultValueWidget()
{
	EVisibility Visibility = EVisibility::Collapsed;

	if (GraphPinObj->Direction == EEdGraphPinDirection::EGPD_Input && GraphPinObj->LinkedTo.Num() == 0)
	{
		Visibility = EVisibility::Visible;
	}

	return SNew(STextComboBox)

		// Setting source of options available in combo box widget
		.OptionsSource(&TeamsList)

		// Singing up for event fired whenever user selects an option from combo box.
		.OnSelectionChanged(this, &STeamPin::OnTeamSelected);

		//.Visibility(Visibility);
}

void STeamPin::OnTeamSelected(TSharedPtr<FString> InSelectedString, ESelectInfo::Type InSelectionType)
{
	FString CurrentDefaultValue = GraphPinObj->GetDefaultAsString();

	FString SelectedTeamName = *InSelectedString;

	if (CurrentDefaultValue.IsEmpty())
	{
		CurrentDefaultValue = FString(TEXT("Purple"));
	}

	// Setting up 
	FString TeamNameString = TEXT("(");
	if (!SelectedTeamName.IsEmpty())
	{
		//now set here proerty name from USTRUCT(), \" - will add opening "
		// so it will look like AttributeName="

		TeamNameString += TEXT("Name=\"");
		//add value you want to set to your property"
		TeamNameString += SelectedTeamName;
		//close with "
		TeamNameString += TEXT("\"");
	}

	//and at last add ) so it will look like (AttributeName="Value");
	TeamNameString += TEXT(")");

    //and here we set our value to parameter if it different than last one.
	if (!CurrentDefaultValue.Equals(TeamNameString))
	{
		auto* schema = GraphPinObj->GetSchema();

		GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, TeamNameString);
	}
}

void STeamPin::OnGraphChanged(const FEdGraphEditAction& InAction)
{
	EVisibility Visibility = EVisibility::Collapsed;
	if (GraphPinObj->Direction == EEdGraphPinDirection::EGPD_Input && GraphPinObj->LinkedTo.Num() == 0)
	{
		Visibility = EVisibility::Visible;
	}

	SetVisibility(Visibility);
}