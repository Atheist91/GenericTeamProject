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

	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;

	void OnTeamSelected(TSharedPtr<FString> InSelectedString, ESelectInfo::Type InSelectionType);
};