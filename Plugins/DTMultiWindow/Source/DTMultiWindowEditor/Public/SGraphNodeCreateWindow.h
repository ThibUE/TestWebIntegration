// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#pragma once

#include "KismetNodes/SGraphNodeK2Default.h"
#include "Templates/SharedPointer.h"

class SGraphPin;
class UEdGraphPin;

class SGraphNodeCreateWindow : public SGraphNodeK2Default
{
public:
	// SGraphNode interface
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;
	virtual void OnAdvancedViewChanged( const ECheckBoxState NewCheckedState ) override;
	// End of SGraphNode interface
};

