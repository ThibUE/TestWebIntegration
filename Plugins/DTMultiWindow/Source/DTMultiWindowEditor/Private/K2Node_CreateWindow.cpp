// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "K2Node_CreateWindow.h"

#if WITH_EDITOR
	#include "GameFramework/PlayerController.h"
#endif // WITH_EDITOR
#include "DTMultiWindowBPLib.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "Blueprint/UserWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "KismetCompilerMisc.h"
#include "KismetCompiler.h"
#include "EditorCategoryUtils.h"

#include "SGraphNodeCreateWindow.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "FDTMultiWindowEditorModule"


struct FK2Node_CreateWindowHelper
{
	static const FName WorldContextObject;
	static const FName Class;
	static const FName Title;
	static const FName ClientSize;
	static const FName DoModal;
	static const FName Show;
	static const FName WindowType;
	static const FName ScreenPosition;
	static const FName AutoCenter;
	static const FName DragAnywhere;
	static const FName IsInitiallyMaximized;
	static const FName IsInitiallyMinimized;
	static const FName SizingRule;
	static const FName IsTopmostWindow;
	static const FName FocusWhenFirstShown;
	static const FName HasCloseButton;
	static const FName SupportsMaximize;
	static const FName SupportsMinimize;
	static const FName ShouldPreserveAspectRatio;
	static const FName MinWidth;
	static const FName MinHeight;
	static const FName MaxWidth;
	static const FName MaxHeight;
};

const FName FK2Node_CreateWindowHelper::WorldContextObject(TEXT("WorldContextObject"));
const FName FK2Node_CreateWindowHelper::Class(TEXT("Class"));
const FName FK2Node_CreateWindowHelper::Title(TEXT("Title"));
const FName FK2Node_CreateWindowHelper::ClientSize(TEXT("ClientSize"));
const FName FK2Node_CreateWindowHelper::DoModal(TEXT("DoModal"));
const FName FK2Node_CreateWindowHelper::Show(TEXT("Show"));
const FName FK2Node_CreateWindowHelper::WindowType(TEXT("WindowType"));
const FName FK2Node_CreateWindowHelper::ScreenPosition(TEXT("ScreenPosition"));
const FName FK2Node_CreateWindowHelper::AutoCenter(TEXT("AutoCenter"));
const FName FK2Node_CreateWindowHelper::DragAnywhere(TEXT("DragAnywhere"));
const FName FK2Node_CreateWindowHelper::IsInitiallyMaximized(TEXT("IsInitiallyMaximized"));
const FName FK2Node_CreateWindowHelper::IsInitiallyMinimized(TEXT("IsInitiallyMinimized"));
const FName FK2Node_CreateWindowHelper::SizingRule(TEXT("SizingRule"));
const FName FK2Node_CreateWindowHelper::IsTopmostWindow(TEXT("IsTopmostWindow"));
const FName FK2Node_CreateWindowHelper::FocusWhenFirstShown(TEXT("FocusWhenFirstShown"));
const FName FK2Node_CreateWindowHelper::HasCloseButton(TEXT("HasCloseButton"));
const FName FK2Node_CreateWindowHelper::SupportsMaximize(TEXT("SupportsMaximize"));
const FName FK2Node_CreateWindowHelper::SupportsMinimize(TEXT("SupportsMinimize"));
const FName FK2Node_CreateWindowHelper::ShouldPreserveAspectRatio(TEXT("ShouldPreserveAspectRatio"));
const FName FK2Node_CreateWindowHelper::MinWidth(TEXT("MinWidth"));
const FName FK2Node_CreateWindowHelper::MinHeight(TEXT("MinHeight"));
const FName FK2Node_CreateWindowHelper::MaxWidth(TEXT("MaxWidth"));
const FName FK2Node_CreateWindowHelper::MaxHeight(TEXT("MaxHeight"));

UK2Node_CreateWindow::UK2Node_CreateWindow(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("NodeTooltip", "Window is a platform-agnostic representation of a top-level window. \n\n"
	"Class : The type of DT User Widget. \n"
	"Title : Title of the window.\n"
	"ClientSize : What the initial size of the window should be.\n"
	"DoModal : Modal window.\n"
	"Show : Show window.\n"
	"WindowType : Type of this window.\n"
	"ScreenPosition : Screen-space position where the window should be initially located.\n"
	"AutoCenter : The windows auto-centering mode. If set to anything other than None, then the ScreenPosition value will be ignored.\n"
	"DragAnywhere : The windows auto-ceWhen true, the window can be dragged from anywhere.\n"
	"IsInitiallyMaximized : Is the window initially maximized.\n"
	"IsInitiallyMinimized : Is the window initially minimized.\n"
	"SizingRule : How the window should be sized.\n"
	"IsTopmostWindow : True if this window should always be on top of all other windows.\n"
	"FocusWhenFirstShown : Should this window be focused immediately after it is shown?\n"
	"HasCloseButton : Does this window have a close button?\n"
	"SupportsMaximize : Can this window be maximized?\n"
	"SupportsMinimize : Can this window be minimized?\n"
	"ShouldPreserveAspectRatio : Should this window preserve its aspect ratio when resized by user? \n"
	"MinWidth : The smallest width this window can be in Desktop Pixel Units.\n"
	"MinHeight : The smallest height this window can be in Desktop Pixel Units.\n"
	"MaxWidth : The biggest width this window can be in Desktop Pixel Units.\n"
	"MaxHeight : The biggest height this window can be in Desktop Pixel Units.");
}

void UK2Node_CreateWindow::PostLoad()
{
	Super::PostLoad();
}

void UK2Node_CreateWindow::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	
	// 创建输入节点
	UEdGraphPin* PinTitle = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Text, FK2Node_CreateWindowHelper::Title);
	UEdGraphPin* PinClientSize = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TBaseStructure<FVector2D>::Get(), FK2Node_CreateWindowHelper::ClientSize);
	UEdGraphPin* PinDoModal = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FK2Node_CreateWindowHelper::DoModal);
	UEdGraphPin* PinShow = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FK2Node_CreateWindowHelper::Show);
	UEdGraphPin* PinWindowType = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EDTWindowType>(), FK2Node_CreateWindowHelper::WindowType);
	UEdGraphPin* PinScreenPosition = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TBaseStructure<FVector2D>::Get(), FK2Node_CreateWindowHelper::ScreenPosition);
	UEdGraphPin* PinAutoCenter = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EDTAutoCenter>(), FK2Node_CreateWindowHelper::AutoCenter);
	UEdGraphPin* PinDragAnywhere = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FK2Node_CreateWindowHelper::DragAnywhere);
	UEdGraphPin* PinIsInitiallyMaximized = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FK2Node_CreateWindowHelper::IsInitiallyMaximized);
	UEdGraphPin* PinIsInitiallyMinimized = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FK2Node_CreateWindowHelper::IsInitiallyMinimized);
	UEdGraphPin* PinSizingRule = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, StaticEnum<EDTSizingRule>(), FK2Node_CreateWindowHelper::SizingRule);
	UEdGraphPin* PinIsTopmostWindow = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FK2Node_CreateWindowHelper::IsTopmostWindow);
	UEdGraphPin* PinFocusWhenFirstShown = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FK2Node_CreateWindowHelper::FocusWhenFirstShown);
	UEdGraphPin* PinHasCloseButton = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FK2Node_CreateWindowHelper::HasCloseButton);
	UEdGraphPin* PinSupportsMaximize = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FK2Node_CreateWindowHelper::SupportsMaximize);
	UEdGraphPin* PinSupportsMinimize = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FK2Node_CreateWindowHelper::SupportsMinimize);
	UEdGraphPin* PinShouldPreserveAspectRatio = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, FK2Node_CreateWindowHelper::ShouldPreserveAspectRatio);
	UEdGraphPin* PinMinWidth = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Float, FK2Node_CreateWindowHelper::MinWidth);
	UEdGraphPin* PinMinHeight = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Float, FK2Node_CreateWindowHelper::MinHeight);
	UEdGraphPin* PinMaxWidth = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Float, FK2Node_CreateWindowHelper::MaxWidth);
	UEdGraphPin* PinMaxHeight = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Real, UEdGraphSchema_K2::PC_Float, FK2Node_CreateWindowHelper::MaxHeight);

	// 设置默认属性
	FString ClientSizeDefaultString;
	const FVector2D ClientSizeDefault(800, 600);
	TBaseStructure<FVector2D>::Get()->ExportText(ClientSizeDefaultString, &ClientSizeDefault, nullptr, nullptr, PPF_None, nullptr);
	K2Schema->SetPinDefaultValueAtConstruction(PinClientSize, ClientSizeDefaultString);
	K2Schema->SetPinAutogeneratedDefaultValue(PinClientSize,  ClientSizeDefaultString);
	K2Schema->SetPinDefaultValueAtConstruction(PinShow,TEXT("true"));
	K2Schema->SetPinAutogeneratedDefaultValue(PinShow,TEXT("true"));
	K2Schema->SetPinDefaultValueAtConstruction(PinWindowType, TEXT("Normal"));
	K2Schema->SetPinAutogeneratedDefaultValue(PinWindowType, TEXT("Normal"));
	K2Schema->SetPinDefaultValueAtConstruction(PinAutoCenter, TEXT("PreferredWorkArea"));
	K2Schema->SetPinAutogeneratedDefaultValue(PinAutoCenter, TEXT("PreferredWorkArea"));
	K2Schema->SetPinDefaultValueAtConstruction(PinSizingRule, TEXT("FixedSize"));
	K2Schema->SetPinAutogeneratedDefaultValue(PinSizingRule, TEXT("FixedSize"));
	K2Schema->SetPinDefaultValueAtConstruction(PinFocusWhenFirstShown, TEXT("true"));
	K2Schema->SetPinAutogeneratedDefaultValue(PinFocusWhenFirstShown, TEXT("true"));
	K2Schema->SetPinDefaultValueAtConstruction(PinHasCloseButton, TEXT("true"));
	K2Schema->SetPinAutogeneratedDefaultValue(PinHasCloseButton, TEXT("true"));
	K2Schema->SetPinDefaultValueAtConstruction(PinSupportsMaximize, TEXT("true"));
	K2Schema->SetPinAutogeneratedDefaultValue(PinSupportsMaximize, TEXT("true"));
	K2Schema->SetPinDefaultValueAtConstruction(PinSupportsMinimize,TEXT("true"));
	K2Schema->SetPinAutogeneratedDefaultValue(PinSupportsMinimize,TEXT("true"));
	
	// 设置提示信息
	SetPinToolTip(*PinTitle, LOCTEXT("ToolTipTitle", "Title of the window."));
	SetPinToolTip(*PinClientSize, LOCTEXT("ToolTipClientSize", "What the initial size of the window should be.\r\rMust: SizingRule is not Autosized"));
	SetPinToolTip(*PinDoModal, LOCTEXT("ToolTipDoModal", "Modal window."));
	SetPinToolTip(*PinDoModal, LOCTEXT("ToolTipDoModal", "Show window."));
	SetPinToolTip(*PinWindowType, LOCTEXT("ToolTipWindowType", "Type of this window."));
	SetPinToolTip(*PinScreenPosition, LOCTEXT("ToolTipScreenPosition", "Screen-space position where the window should be initially located.\r\rMust: AutoCenter is None"));
	SetPinToolTip(*PinAutoCenter, LOCTEXT("ToolTipAutoCenter", "The windows auto-centering mode. If set to anything other than None, then the ScreenPosition value will be ignored."));
	SetPinToolTip(*PinDragAnywhere, LOCTEXT("ToolTipDragAnywhere", "The windows auto-ceWhen true, the window can be dragged from anywhere."));
	SetPinToolTip(*PinIsInitiallyMaximized, LOCTEXT("ToolTipIsInitiallyMaximized", "Is the window initially maximized."));
	SetPinToolTip(*PinIsInitiallyMinimized, LOCTEXT("ToolTipIsInitiallyMinimized", "Is the window initially minimized."));
	SetPinToolTip(*PinSizingRule, LOCTEXT("ToolTipSizingRule", "How the window should be sized."));
	SetPinToolTip(*PinIsTopmostWindow, LOCTEXT("ToolTipIsTopmostWindow", "True if this window should always be on top of all other windows."));
	SetPinToolTip(*PinFocusWhenFirstShown, LOCTEXT("ToolTipFocusWhenFirstShown", "Should this window be focused immediately after it is shown?"));
	SetPinToolTip(*PinHasCloseButton, LOCTEXT("ToolTipHasCloseButton", "Does this window have a close button?\r\rMust: WindowType is Normal"));
	SetPinToolTip(*PinSupportsMaximize, LOCTEXT("ToolTipSupportsMaximize", "Can this window be maximized?\r\rMust: DoModal is false And WindowType is Normal"));
	SetPinToolTip(*PinSupportsMinimize, LOCTEXT("ToolTipSupportsMinimize", "Can this window be minimized?\r\rMust: DoModal is false And WindowType is Normal"));
	SetPinToolTip(*PinShouldPreserveAspectRatio, LOCTEXT("ToolTipShouldPreserveAspectRatio", "Should this window preserve its aspect ratio when resized by user? \r\rMust: SizingRule is UserSized"));
	SetPinToolTip(*PinMinWidth, LOCTEXT("ToolTipMinWidth", "The smallest width this window can be in Desktop Pixel Units.\rNon -0 effective"));
	SetPinToolTip(*PinMinHeight, LOCTEXT("ToolTipMinHeight", "The smallest height this window can be in Desktop Pixel Units.\rNon -0 effective"));
	SetPinToolTip(*PinMaxWidth, LOCTEXT("ToolTipMaxWidth", "The biggest width this window can be in Desktop Pixel Units.\rNon -0 effective"));
	SetPinToolTip(*PinMaxHeight, LOCTEXT("ToolTipMaxHeight", "The biggest height this window can be in Desktop Pixel Units.\rNon -0 effective"));

	// 添加需要隐藏的变量
	AdvancedPinDisplay = ENodeAdvancedPins::Hidden;
	AdvancedPins.Add(PinDoModal);
	AdvancedPins.Add(PinShow);
	AdvancedPins.Add(PinWindowType);
	AdvancedPins.Add(PinScreenPosition);
	AdvancedPins.Add(PinAutoCenter);
	AdvancedPins.Add(PinDragAnywhere);
	AdvancedPins.Add(PinIsInitiallyMaximized);
	AdvancedPins.Add(PinIsInitiallyMinimized);
	AdvancedPins.Add(PinSizingRule);
	AdvancedPins.Add(PinIsTopmostWindow);
	AdvancedPins.Add(PinFocusWhenFirstShown);
	AdvancedPins.Add(PinHasCloseButton);
	AdvancedPins.Add(PinSupportsMaximize);
	AdvancedPins.Add(PinSupportsMinimize);
	AdvancedPins.Add(PinShouldPreserveAspectRatio);
	AdvancedPins.Add(PinMinWidth);
	AdvancedPins.Add(PinMinHeight);
	AdvancedPins.Add(PinMaxWidth);
	AdvancedPins.Add(PinMaxHeight);
	for ( UEdGraphPin * Pin : AdvancedPins )
	{
		Pin->SafeSetHidden(AdvancedPinDisplay == ENodeAdvancedPins::Hidden);
	}
}

void UK2Node_CreateWindow::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
}

FText UK2Node_CreateWindow::GetBaseNodeTitle() const
{
	return LOCTEXT("CreateWindow_BaseTitle", "Create Window");
}

FText UK2Node_CreateWindow::GetDefaultNodeTitle() const
{
	return LOCTEXT("CreateWindow_NONE", "Create NONE Window");
}

FText UK2Node_CreateWindow::GetNodeTitleFormat() const
{
	return LOCTEXT("CreateWindow", "Create {ClassName} Window");
}

UClass* UK2Node_CreateWindow::GetClassPinBaseClass() const
{
	return UDTUserWidget::StaticClass();
}

FText UK2Node_CreateWindow::GetMenuCategory() const
{
	return FText::FromString(TEXT("User Interface|DT Multi Window"));
}

FName UK2Node_CreateWindow::GetCornerIcon() const
{
	return TEXT("Graph.Replication.ClientEvent");
}

// 生成后更新数据
void UK2Node_CreateWindow::PostReconstructNode()
{
	Super::PostReconstructNode();

	// 模态窗口修改
	PinDoModalChange(FindPinChecked(FK2Node_CreateWindowHelper::DoModal, EGPD_Input));
	
	// 窗口类型
	PinWindowTypeChange(FindPinChecked(FK2Node_CreateWindowHelper::WindowType, EGPD_Input));
	
	// 居中模式修改
	PinAutoCenterChange(FindPinChecked(FK2Node_CreateWindowHelper::AutoCenter, EGPD_Input));

	// 窗口大小模式改变
	PinSizingRuleChange(FindPinChecked(FK2Node_CreateWindowHelper::SizingRule, EGPD_Input));
}

bool UK2Node_CreateWindow::IsSpawnVarPin(UEdGraphPin* Pin) const
{
	return(
		Super::IsSpawnVarPin(Pin)
		&& Pin->PinName != FK2Node_CreateWindowHelper::Title
		&& Pin->PinName != FK2Node_CreateWindowHelper::ClientSize
		&& Pin->PinName != FK2Node_CreateWindowHelper::DoModal
		&& Pin->PinName != FK2Node_CreateWindowHelper::Show
		&& Pin->PinName != FK2Node_CreateWindowHelper::WindowType
		&& Pin->PinName != FK2Node_CreateWindowHelper::ScreenPosition
		&& Pin->PinName != FK2Node_CreateWindowHelper::AutoCenter
		&& Pin->PinName != FK2Node_CreateWindowHelper::DragAnywhere
		&& Pin->PinName != FK2Node_CreateWindowHelper::IsInitiallyMaximized
		&& Pin->PinName != FK2Node_CreateWindowHelper::IsInitiallyMinimized
		&& Pin->PinName != FK2Node_CreateWindowHelper::SizingRule
		&& Pin->PinName != FK2Node_CreateWindowHelper::IsTopmostWindow
		&& Pin->PinName != FK2Node_CreateWindowHelper::FocusWhenFirstShown
		&& Pin->PinName != FK2Node_CreateWindowHelper::HasCloseButton
		&& Pin->PinName != FK2Node_CreateWindowHelper::SupportsMaximize
		&& Pin->PinName != FK2Node_CreateWindowHelper::SupportsMinimize
		&& Pin->PinName != FK2Node_CreateWindowHelper::ShouldPreserveAspectRatio
		&& Pin->PinName != FK2Node_CreateWindowHelper::MinWidth
		&& Pin->PinName != FK2Node_CreateWindowHelper::MinHeight
		&& Pin->PinName != FK2Node_CreateWindowHelper::MaxWidth
		&& Pin->PinName != FK2Node_CreateWindowHelper::MaxHeight
		 );
}



void UK2Node_CreateWindow::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
	
	static const FName Create_FunctionName = GET_FUNCTION_NAME_CHECKED(UDTMultiWindowBPLib, CreateWindow);
	UK2Node_CreateWindow* CreateWindowNode = this;
	UEdGraphPin* SpawnNodeExec = CreateWindowNode->GetExecPin();
	UEdGraphPin* SpawnWorldContextPin = CreateWindowNode->GetWorldContextPin();
	UEdGraphPin* SpawnClassPin = CreateWindowNode->GetClassPin();
	UEdGraphPin* SpawnNodeThen = CreateWindowNode->GetThenPin();
	UEdGraphPin* SpawnNodeResult = CreateWindowNode->GetResultPin();
	UClass* SpawnClass = ( SpawnClassPin != NULL ) ? Cast<UClass>(SpawnClassPin->DefaultObject) : NULL;
	if ( !SpawnClassPin || ((0 == SpawnClassPin->LinkedTo.Num()) && (NULL == SpawnClass)))
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("CreateWindowNodeMissingClass_Error", "Spawn node @@ must have a class specified.").ToString(), CreateWindowNode);
		// we break exec links so this is the only error we get, don't want the CreateWidget node being considered and giving 'unexpected node' type warnings
		CreateWindowNode->BreakAllNodeLinks();
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// create 'UDTMultiWindowBPLib::CreateWindow' call node
	UK2Node_CallFunction* CallCreateNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(CreateWindowNode, SourceGraph);
	CallCreateNode->FunctionReference.SetExternalMember(Create_FunctionName, UWidgetBlueprintLibrary::StaticClass());
	CallCreateNode->AllocateDefaultPins();

	// store off the class to spawn before we mutate pin connections:
	UClass* ClassToSpawn = GetClassToSpawn();
	UEdGraphPin* CallCreateExec = CallCreateNode->GetExecPin();
	UEdGraphPin* CallCreateWidgetTypePin = CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::Class);
	UEdGraphPin* CallCreateResult = CallCreateNode->GetReturnValuePin();

	// Move 'exec' connection from create widget node to 'UDTMultiWindowBPLib::CreateWindow'
	CompilerContext.MovePinLinksToIntermediate(*SpawnNodeExec, *CallCreateExec);

	if ( SpawnClassPin->LinkedTo.Num() > 0 )
	{
		// Copy the 'blueprint' connection from the spawn node to 'UDTMultiWindowBPLib::CreateWindow'
		CompilerContext.MovePinLinksToIntermediate(*SpawnClassPin, *CallCreateWidgetTypePin);
	}
	else
	{
		// Copy blueprint literal onto 'UDTMultiWindowBPLib::CreateWindow' call
		CallCreateWidgetTypePin->DefaultObject = SpawnClass;
	}

	// Copy the world context connection from the spawn node to 'UDTMultiWindowBPLib::CreateWindow' if necessary
	if ( SpawnWorldContextPin )
	{
		CompilerContext.MovePinLinksToIntermediate(*SpawnWorldContextPin, *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::WorldContextObject));
	}

	// Copy parameter connection from the spawn node to 'UDTMultiWindowBPLib::CreateWindow'
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::Title, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::Title));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::ClientSize, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::ClientSize));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::DoModal, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::DoModal));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::Show, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::Show));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::WindowType, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::WindowType));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::ScreenPosition, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::ScreenPosition));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::AutoCenter, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::AutoCenter));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::DragAnywhere, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::DragAnywhere));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::IsInitiallyMaximized, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::IsInitiallyMaximized));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::IsInitiallyMinimized, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::IsInitiallyMinimized));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::SizingRule, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::SizingRule));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::IsTopmostWindow, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::IsTopmostWindow));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::FocusWhenFirstShown, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::FocusWhenFirstShown));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::HasCloseButton, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::HasCloseButton));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::SupportsMaximize, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::SupportsMaximize));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::SupportsMinimize, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::SupportsMinimize));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::ShouldPreserveAspectRatio, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::ShouldPreserveAspectRatio));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::MinWidth, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::MinWidth));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::MinHeight, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::MinHeight));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::MaxWidth, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::MaxWidth));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(FK2Node_CreateWindowHelper::MaxHeight, EGPD_Input), *CallCreateNode->FindPinChecked(FK2Node_CreateWindowHelper::MaxHeight));

	// Move result connection from spawn node to 'UDTMultiWindowBPLib::CreateWindow'
	CallCreateResult->PinType = SpawnNodeResult->PinType; // Copy type so it uses the right actor subclass
	CompilerContext.MovePinLinksToIntermediate(*SpawnNodeResult, *CallCreateResult);

	//////////////////////////////////////////////////////////////////////////
	// create 'set var' nodes

	// Get 'result' pin from 'begin spawn', this is the actual actor we want to set properties on
	UEdGraphPin* LastThen = FKismetCompilerUtilities::GenerateAssignmentNodes(CompilerContext, SourceGraph, CallCreateNode, CreateWindowNode, CallCreateResult, ClassToSpawn);

	// Move 'then' connection from create widget node to the last 'then'
	CompilerContext.MovePinLinksToIntermediate(*SpawnNodeThen, *LastThen);

	// Break any links to the expanded node
	CreateWindowNode->BreakAllNodeLinks();
}

TSharedPtr<SGraphNode> UK2Node_CreateWindow::CreateVisualWidget()
{
	return SNew(SGraphNodeCreateWindow, this);
}

void UK2Node_CreateWindow::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::PinDefaultValueChanged(Pin);

	// 是否通知刷新
	bool bGraphChanged = false;

	// 模态窗口修改
	bGraphChanged = PinDoModalChange(Pin) ? true : bGraphChanged;

	// 窗口类型
	bGraphChanged = PinWindowTypeChange(Pin) ? true : bGraphChanged;
	
	// 居中模式修改
	bGraphChanged = PinAutoCenterChange(Pin) ? true : bGraphChanged;

	// 窗口大小模式改变
	bGraphChanged = PinSizingRuleChange(Pin) ? true : bGraphChanged;

	// MinMax 状态改变
	bGraphChanged = PinMinMaxChange(Pin) ? true : bGraphChanged;

	// 刷新蓝图
	if ( bGraphChanged )
	{
		GetGraph()->NotifyGraphChanged();
	}
}

void UK2Node_CreateWindow::SwitchAdvancedPinsHidden()
{
	// 遍历需要隐藏的按钮
	for ( UEdGraphPin * Pin : AdvancedPins )
	{
		Pin->SafeSetHidden(AdvancedPinDisplay == ENodeAdvancedPins::Hidden);
	}

	// 标记蓝图刷新
	GetGraph()->NotifyGraphChanged();
}

// DoModal 状态改变
bool UK2Node_CreateWindow::PinDoModalChange(UEdGraphPin* Pin)
{
	if ( Pin == nullptr || !Pin->PinName.IsEqual(FK2Node_CreateWindowHelper::DoModal))
	{
		return false;
	}

	// 是否有修改
	bool bIsModify = false;
	
	// 开启模态窗口则没有最大最小化， 而且必须是显示状态
	UEdGraphPin* PinShow = FindPinChecked(FK2Node_CreateWindowHelper::Show, EGPD_Input);
	UEdGraphPin* PinSupportsMaximize = FindPinChecked(FK2Node_CreateWindowHelper::SupportsMaximize, EGPD_Input);
	UEdGraphPin* PinSupportsMinimize = FindPinChecked(FK2Node_CreateWindowHelper::SupportsMinimize, EGPD_Input);
	if ( Pin->DefaultValue.Equals(TEXT("true"), ESearchCase::IgnoreCase) )
	{
		bIsModify = DisablePin(PinShow, TEXT("true")) ? true : bIsModify;
		bIsModify = DisablePin(PinSupportsMaximize, TEXT("false")) ? true : bIsModify;
		bIsModify = DisablePin(PinSupportsMinimize, TEXT("false")) ? true : bIsModify;
	}
	else
	{
		bIsModify = RevertPin(PinShow) ? true : bIsModify;
		bIsModify = RevertPin(PinSupportsMaximize) ? true : bIsModify;
		bIsModify = RevertPin(PinSupportsMinimize) ? true : bIsModify;
	}

	return bIsModify;
}

// WindowType 状态改变
bool UK2Node_CreateWindow::PinWindowTypeChange(UEdGraphPin* Pin)
{
	if ( Pin == nullptr || !Pin->PinName.IsEqual(FK2Node_CreateWindowHelper::WindowType))
	{
		return false;
	}

	// 是否有修改
	bool bIsModify = false;
	
	// 无边框窗口类型， 没有最小化，最大化，关闭按钮
	UEdGraphPin* PinHasCloseButton = FindPinChecked(FK2Node_CreateWindowHelper::HasCloseButton, EGPD_Input);
	UEdGraphPin* PinSupportsMaximize = FindPinChecked(FK2Node_CreateWindowHelper::SupportsMaximize, EGPD_Input);
	UEdGraphPin* PinSupportsMinimize = FindPinChecked(FK2Node_CreateWindowHelper::SupportsMinimize, EGPD_Input);
	if ( Pin->DefaultValue.Equals(TEXT("NoBorder"), ESearchCase::IgnoreCase) )
	{
		bIsModify = DisablePin(PinHasCloseButton, TEXT("false")) ? true : bIsModify;
		bIsModify = DisablePin(PinSupportsMaximize, TEXT("false")) ? true : bIsModify;
		bIsModify = DisablePin(PinSupportsMinimize, TEXT("false")) ? true : bIsModify;
	}
	else
	{
		bIsModify = RevertPin(PinHasCloseButton) ? true : bIsModify;
		bIsModify = RevertPin(PinSupportsMaximize) ? true : bIsModify;
		bIsModify = RevertPin(PinSupportsMinimize) ? true : bIsModify;
	}

	return bIsModify;
}

// AutoCenter 状态改变
bool UK2Node_CreateWindow::PinAutoCenterChange(UEdGraphPin* Pin)
{
	if ( Pin == nullptr || !Pin->PinName.IsEqual(FK2Node_CreateWindowHelper::AutoCenter))
	{
		return false;
	}

	// 是否有修改
	bool bIsModify = false;
	
	// 非无效模式，需要屏蔽位置
	UEdGraphPin* PinScreenPosition = FindPinChecked(FK2Node_CreateWindowHelper::ScreenPosition, EGPD_Input);
	if ( !Pin->DefaultValue.Equals(TEXT("None"), ESearchCase::IgnoreCase) )
	{
		FString ClientSizeString;
		const FVector2D ClientSize(0, 0);
		TBaseStructure<FVector2D>::Get()->ExportText(ClientSizeString, &ClientSize, nullptr, nullptr, PPF_None, nullptr);
		bIsModify = DisablePin(PinScreenPosition, ClientSizeString) ? true : bIsModify;
	}
	else
	{
		bIsModify = RevertPin(PinScreenPosition) ? true : bIsModify;
	}

	return bIsModify;
}

// SizingRule 状态改变
bool UK2Node_CreateWindow::PinSizingRuleChange(UEdGraphPin* Pin)
{
	if ( Pin == nullptr || !Pin->PinName.IsEqual(FK2Node_CreateWindowHelper::SizingRule))
	{
		return false;
	}

	// 是否有修改
	bool bIsModify = false;
	
	// 自动大小模式，需要屏蔽窗口大小
	UEdGraphPin* PinClientSize = FindPinChecked(FK2Node_CreateWindowHelper::ClientSize, EGPD_Input);
	if ( Pin->DefaultValue.Equals(TEXT("Autosized"), ESearchCase::IgnoreCase) )
	{
		FString ClientSizeString;
		const FVector2D ClientSize(0, 0);
		TBaseStructure<FVector2D>::Get()->ExportText(ClientSizeString, &ClientSize, nullptr, nullptr, PPF_None, nullptr);
		bIsModify = DisablePin(PinClientSize, ClientSizeString) ? true : bIsModify;
	}
	else
	{
		bIsModify = RevertPin(PinClientSize) ? true : bIsModify;
	}

	// 非用户可修改模式，屏蔽按比例缩放, 还最大最小窗口
	UEdGraphPin* PinShouldPreserveAspectRatio = FindPinChecked(FK2Node_CreateWindowHelper::ShouldPreserveAspectRatio, EGPD_Input);
	UEdGraphPin* PinMinWidth = FindPinChecked(FK2Node_CreateWindowHelper::MinWidth, EGPD_Input);
	UEdGraphPin* PinMinHeight = FindPinChecked(FK2Node_CreateWindowHelper::MinHeight, EGPD_Input);
	UEdGraphPin* PinMaxWidth = FindPinChecked(FK2Node_CreateWindowHelper::MaxWidth, EGPD_Input);
	UEdGraphPin* PinMaxHeight = FindPinChecked(FK2Node_CreateWindowHelper::MaxHeight, EGPD_Input);
	if ( !Pin->DefaultValue.Equals(TEXT("UserSized"), ESearchCase::IgnoreCase) )
	{
		bIsModify = DisablePin(PinShouldPreserveAspectRatio, TEXT("false")) ? true : bIsModify;
		bIsModify = DisablePin(PinMinWidth, TEXT("0.0")) ? true : bIsModify;
		bIsModify = DisablePin(PinMinHeight, TEXT("0.0")) ? true : bIsModify;
		bIsModify = DisablePin(PinMaxWidth, TEXT("0.0")) ? true : bIsModify;
		bIsModify = DisablePin(PinMaxHeight, TEXT("0.0")) ? true : bIsModify;
	}
	else
	{
		bIsModify = RevertPin(PinShouldPreserveAspectRatio) ? true : bIsModify;
		bIsModify = RevertPin(PinMinWidth) ? true : bIsModify;
		bIsModify = RevertPin(PinMinHeight) ? true : bIsModify;
		bIsModify = RevertPin(PinMaxWidth) ? true : bIsModify;
		bIsModify = RevertPin(PinMaxHeight) ? true : bIsModify;
	}

	return bIsModify;
}

// MinMax 状态改变
bool UK2Node_CreateWindow::PinMinMaxChange(UEdGraphPin* Pin)
{
	if ( Pin == nullptr )
	{
		return false;
	}

	// 是否有修改
	bool bIsModify = false;

	// 操作最小宽度， 限制最大宽度
	if ( Pin->PinName.IsEqual(FK2Node_CreateWindowHelper::MinWidth) )
	{
		float fValue = FCString::Atof(*Pin->DefaultValue);
		if ( fValue < 0.0 )
		{
			fValue = 0.0;
			Pin->DefaultValue = FString::SanitizeFloat(fValue);
			bIsModify = true;
		}
		UEdGraphPin* PinMaxWidth = FindPinChecked(FK2Node_CreateWindowHelper::MaxWidth, EGPD_Input);
		if ( fValue > FCString::Atof(*PinMaxWidth->DefaultValue)  )
		{
			PinMaxWidth->DefaultValue = FString::SanitizeFloat(fValue + 1.f);
			bIsModify = true;
		}
	}
	// 操作最小高度， 限制最大高度
	else if ( Pin->PinName.IsEqual(FK2Node_CreateWindowHelper::MinHeight) )
	{
		float fValue = FCString::Atof(*Pin->DefaultValue);
		if ( fValue < 0.0 )
		{
			fValue = 0.0;
			Pin->DefaultValue = FString::SanitizeFloat(fValue);
			bIsModify = true;
		}
		UEdGraphPin* PinMaxHeight = FindPinChecked(FK2Node_CreateWindowHelper::MaxHeight, EGPD_Input);
		if ( fValue > FCString::Atof(*PinMaxHeight->DefaultValue)  )
		{
			PinMaxHeight->DefaultValue = FString::SanitizeFloat(fValue + 1.f);
			bIsModify = true;
		}
	}
	// 操作最大宽度， 限制最小宽度
	else if ( Pin->PinName.IsEqual(FK2Node_CreateWindowHelper::MaxWidth) )
	{
		float fValue = FCString::Atof(*Pin->DefaultValue);
		if ( fValue < 0.0 )
		{
			fValue = 0.0;
			Pin->DefaultValue = FString::SanitizeFloat(fValue);
			bIsModify = true;
		}
		UEdGraphPin* PinMinWidth = FindPinChecked(FK2Node_CreateWindowHelper::MinWidth, EGPD_Input);
		if ( fValue < FCString::Atof(*PinMinWidth->DefaultValue)  )
		{
			PinMinWidth->DefaultValue = FString::SanitizeFloat(FMath::Max(0.f, fValue - 1.f));
			bIsModify = true;
		}
	}
	// 操作最大高度， 限制最小高度
	else if ( Pin->PinName.IsEqual(FK2Node_CreateWindowHelper::MaxHeight) )
	{
		float fValue = FCString::Atof(*Pin->DefaultValue);
		if ( fValue < 0.0 )
		{
			fValue = 0.0;
			Pin->DefaultValue = FString::SanitizeFloat(fValue);
			bIsModify = true;
		}
		UEdGraphPin* PinMinHeight = FindPinChecked(FK2Node_CreateWindowHelper::MinHeight, EGPD_Input);
		if ( fValue < FCString::Atof(*PinMinHeight->DefaultValue)  )
		{
			PinMinHeight->DefaultValue = FString::SanitizeFloat(FMath::Max(0.f, fValue - 1.f));
			bIsModify = true;
		}
	}

	return bIsModify;
}

// 把节点设置值并禁用节点
bool UK2Node_CreateWindow::DisablePin(UEdGraphPin* Pin, const FString& Value)
{
	// 当前已经是禁用状态了， 无需要设置了
	if ( Pin->bDefaultValueIsReadOnly && Pin->bNotConnectable )
	{
		return false;
	}

	// 保存值
	MapLastValue.Add( Pin->PinName.ToString(), Pin->DefaultValue );

	// 设置状态
	Pin->bDefaultValueIsReadOnly = true;
	Pin->bNotConnectable = true;
	Pin->DefaultValue = Value;
	
	return true;
}

bool UK2Node_CreateWindow::RevertPin(UEdGraphPin* Pin)
{
	// 当前已经是可用状态，不需要还原
	if ( !Pin->bDefaultValueIsReadOnly && !Pin->bNotConnectable )
	{
		return false;
	}

	// 读取保存值
	FString Value;
	MapLastValue.RemoveAndCopyValue(Pin->PinName.ToString(), Value);
	
	// 设置状态
	Pin->bDefaultValueIsReadOnly = false;
	Pin->bNotConnectable = false;
	Pin->DefaultValue = Value;

	return true;
}

#undef LOCTEXT_NAMESPACE
