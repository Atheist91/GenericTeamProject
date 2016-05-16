#pragma once
#include "SlateBasics.h"
#include "SGraphPin.h"
 
class STeamPin : public SGraphPin
{

public:
	SLATE_BEGIN_ARGS(STeamPin) {}
	SLATE_END_ARGS()

protected:

	TArray<TSharedPtr<FString>> TeamsList;
 
public:	

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

	virtual void OnTeamSelected(TSharedPtr<FString> InSelectedString, ESelectInfo::Type InSelectionType);

	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;

	// Determines in what scenario ComboBox is supposed to be visible or not.
	virtual EVisibility GetComboBoxVisibility() const;

protected:

	// Creates string that is ready to put in pin, so it'll override the content of FTeam.Name property.
	FString GetPreparedString(FString InTeamName) const;
};