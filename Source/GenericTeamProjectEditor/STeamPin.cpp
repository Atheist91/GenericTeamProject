#include "GenericTeamProjectEditor.h" 
#include "KismetEditorUtilities.h" 
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "SGraphPin.h"
#include "SNameComboBox.h"
#include "STextComboBox.h"

#include "STeamPin.h"
#include "CustomTypes.h"

void STeamPin::Construct(const FArguments & InArgs, UEdGraphPin * InGraphPinObj)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

void STeamPin::OnTeamSelected(TSharedPtr<FString> InSelectedString, ESelectInfo::Type InSelectionType)
{
	FString Name = InSelectedString.IsValid() ? *InSelectedString : FString("");
	if (auto Schema = (GraphPinObj ? GraphPinObj->GetSchema() : nullptr))
	{
		// If we selected different Team from the ComboBox
		if (GraphPinObj->GetDefaultAsString() != Name)
		{
			// Getting prepared string for 
			FString TeamNameString = GetPreparedString(Name);

			// Editing variable under pin
			GraphPinObj->Modify();
			Schema->TrySetDefaultValue(*GraphPinObj, TeamNameString);
		}
	}
}

TSharedRef<SWidget>	STeamPin::GetDefaultValueWidget()
{
	// Will contain currently selected Name
	TSharedPtr<FString> CurrentlySelectedName;

	// Building list of available names in drop down
	{
		// Getting DataTable containing info about Teams
		UDataTable* DT = UCustomTypes::GetTeamsDataTable();
		if (DT)
		{
			// Getting previously selected string.
			// It'll be formatted as PreparedString (see GetPreparedString function).
			FString PreviousSelection = GraphPinObj->GetDefaultAsString();

			// Building list of options that will be available in combo box widget
			for (FName TeamName : DT->GetRowNames())
			{
				TSharedPtr<FString> TempTeamString = MakeShareable(new FString(TeamName.ToString()));

				// Adding Team to the list
				TeamsList.Add(TempTeamString);

				// Prepared string for current Team
				FString TempPreparedString = GetPreparedString(*TempTeamString.Get());

				// If previously selected Team still exist in available options in ComboBox
				if (PreviousSelection == TempPreparedString)
				{
					CurrentlySelectedName = TempTeamString;
				}
			}
		}
	}

	return SNew(STextComboBox)

		// Aesthetic stuff
		.ContentPadding(FMargin(6.0f, 2.0f))

		// Setting source of available options in combo box. The list is generated above.
		.OptionsSource(&TeamsList)

		// Singing up for event fired whenever user selects an option from combo box.
		.OnSelectionChanged(this, &STeamPin::OnTeamSelected)

		// Setting up what Name is shown as selected initially. 'CurrentlySelectedName' is determined above.
		.InitiallySelectedItem(CurrentlySelectedName)
		
		// Binding visibility of the ComboBox to the given function.
		.Visibility(this, &STeamPin::GetComboBoxVisibility)

		;
}

EVisibility STeamPin::GetComboBoxVisibility() const
{
	// We want to show combo box only, if pin is an input one AND if we didn't plug in any other pins into this one.
	if (GraphPinObj->Direction == EEdGraphPinDirection::EGPD_Input && GraphPinObj->LinkedTo.Num() == 0)
	{
		return EVisibility::Visible;
	}
	else
	{
		return EVisibility::Collapsed;
	}
}

FString STeamPin::GetPreparedString(FString InTeamName) const
{
	return FString::Printf(TEXT("(Name=\"%s\")"), *InTeamName);
}