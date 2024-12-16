// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "K2Node_ConstructObjectFromClass.h"
#include "K2Node_CreateWindow.generated.h"

class UEdGraph;

UCLASS()
class DTMULTIWINDOWEDITOR_API UK2Node_CreateWindow : public UK2Node_ConstructObjectFromClass
{
	GENERATED_UCLASS_BODY()

public:
	TArray<UEdGraphPin*>								AdvancedPins;
	UPROPERTY() TMap<FString, FString>					MapLastValue;

public:
	//~ Begin UObject interface
	virtual void PostLoad() override;
	//~ End UObject Interface.
	
	//~ Begin UEdGraphNode Interface.
	virtual void AllocateDefaultPins() override;
	virtual void PostPlacedNewNode() override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	//~ End UEdGraphNode Interface.

	//~ Begin UK2Node Interface
	virtual FText GetMenuCategory() const override;
	virtual FName GetCornerIcon() const override;
	virtual void PostReconstructNode() override;
	//~ End UK2Node Interface.

	
protected:
	/** Gets the default node title when no class is selected */
	virtual FText GetBaseNodeTitle() const override;
	/** Gets the default node title when no class is selected */
	virtual FText GetDefaultNodeTitle() const;
	/** Gets the node title when a class has been selected. */
	virtual FText GetNodeTitleFormat() const override;
	/** Gets base class to use for the 'class' pin.  UObject by default. */
	virtual UClass* GetClassPinBaseClass() const override;
	/**  */
	virtual bool IsSpawnVarPin(UEdGraphPin* Pin) const override;

public:
	// 却换按钮隐藏
	void SwitchAdvancedPinsHidden();

private:
	// DoModal 状态改变
	bool PinDoModalChange(UEdGraphPin* Pin);
	// WindowType 状态改变
	bool PinWindowTypeChange(UEdGraphPin* Pin);
	// AutoCenter 状态改变
	bool PinAutoCenterChange(UEdGraphPin* Pin);
	// SizingRule 状态改变
	bool PinSizingRuleChange(UEdGraphPin* Pin);
	// MinMax 状态改变
	bool PinMinMaxChange(UEdGraphPin* Pin);
	// 把节点设置值并禁用节点
	bool DisablePin(UEdGraphPin* Pin, const FString & Value);
	// 还原节点
	bool RevertPin(UEdGraphPin* Pin);
};
