// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "SGraphNodeCreateWindow.h"
#include "Containers/Array.h"
#include "EdGraph/EdGraphPin.h"
#include "KismetPins/SGraphPinClass.h"
#include "Misc/AssertionMacros.h"
#include "K2Node_CreateWindow.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Templates/Casts.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SGraphPin;

TSharedPtr<SGraphPin> SGraphNodeCreateWindow::CreatePinWidget(UEdGraphPin* Pin) const
{
	const UK2Node_CreateWindow* CreateWindowNode = CastChecked<UK2Node_CreateWindow>(GraphNode);
	UEdGraphPin* ClassPin = CreateWindowNode->GetClassPin();
	if ((ClassPin == Pin) && (!ClassPin->bHidden || (ClassPin->LinkedTo.Num() > 0)))
	{
		TSharedPtr<SGraphPinClass> NewPin = SNew(SGraphPinClass, ClassPin);
		check(NewPin.IsValid());
		NewPin->SetAllowAbstractClasses(false);
		return NewPin;
	}
	return SGraphNodeK2Default::CreatePinWidget(Pin);
}

void SGraphNodeCreateWindow::OnAdvancedViewChanged(const ECheckBoxState NewCheckedState)
{
	SGraphNodeK2Default::OnAdvancedViewChanged(NewCheckedState);

	UK2Node_CreateWindow* CreateWindowNode = CastChecked<UK2Node_CreateWindow>(GraphNode);
	CreateWindowNode->SwitchAdvancedPinsHidden();
}
