// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "DTMultiWindowEditor.h"

#include "BlueprintActionDatabase.h"
#include "DTMultiWindowBPLib.h"
#include "DTUserWidget.h"
#include "Editor.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "K2Node_GetSubsystem.h"
#include "UMGEditorProjectSettings.h"
#include "AssetTypeActions/AssetTypeActions_DataAsset.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

#define LOCTEXT_NAMESPACE "FDTMultiWindowEditorModule"

#if WITH_EDITOR

EAssetTypeCategories::Type FDTMultiWindowEditorModule::SubsystemAssetCategory;

class FAssetTypeActions_DTUserWidget : public FAssetTypeActions_DataAsset
{
public:
	virtual FText GetName() const override { return LOCTEXT("AssetTypeActions_DTUserWidge", "DT User Widget"); }
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Type::UI; }
	virtual FColor GetTypeColor() const override { return FColor(63, 126, 255); }
	virtual UClass* GetSupportedClass() const override { return UDTUserWidget::StaticClass(); }
};

#endif

// 系统开始运行
void FDTMultiWindowEditorModule::StartupModule()
{
	FEditorDelegates::PreBeginPIE.AddRaw(this, &FDTMultiWindowEditorModule::OnPreBeginPIE);
	FEditorDelegates::EndPIE.AddRaw(this, &FDTMultiWindowEditorModule::OnEndPIE);

#if WITH_EDITOR

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	SubsystemAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("DT Subsystem")), LOCTEXT("DTSubsystemCategory", "DT Subsystem"));
	RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_DTUserWidget()));

#endif
}

// 系统结束运行
void FDTMultiWindowEditorModule::ShutdownModule()
{
#if WITH_EDITOR
	
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (auto CreatedAssetTypeAction : CreatedAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeAction.ToSharedRef());
		}
	}
	CreatedAssetTypeActions.Empty();

#endif
}

// PIE开始
void FDTMultiWindowEditorModule::OnPreBeginPIE(const bool bIsSimulatingInEditor)
{
}

// PIE结束
void FDTMultiWindowEditorModule::OnEndPIE(const bool bIsSimulatingInEditor)
{
	UDTMultiWindowBPLib::CloseAllWindow();
}

#if WITH_EDITOR

// 添加动作
void FDTMultiWindowEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}


// GameInstance 动作工厂
UDTUserWidgetAction_Factory::UDTUserWidgetAction_Factory(const class FObjectInitializer& OBJ) : Super(OBJ)
{
	SupportedClass = UDTUserWidget::StaticClass();
	bEditAfterNew = true;
	bCreateNew = true;
}

// 返回蓝图对象
UObject* UDTUserWidgetAction_Factory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	// Make sure we are trying to factory a blueprint, then create and init one
	check(Class->IsChildOf(UDTUserWidget::StaticClass()));
	
	// If the root widget selection dialog is not enabled, use a canvas panel as the root by default
	UEditorUtilityWidgetBlueprint* NewBP = CastChecked<UEditorUtilityWidgetBlueprint>(FKismetEditorUtilities::CreateBlueprint(Class, InParent, Name, BPTYPE_Normal, UEditorUtilityWidgetBlueprint::StaticClass(), UWidgetBlueprintGeneratedClass::StaticClass(), NAME_None));

	// Create the selected root widget
	if (NewBP->WidgetTree->RootWidget == nullptr)
	{
		if (TSubclassOf<UPanelWidget> RootWidgetPanel = UCanvasPanel::StaticClass())
		{
			UWidget* Root = NewBP->WidgetTree->ConstructWidget<UWidget>(RootWidgetPanel);
			NewBP->WidgetTree->RootWidget = Root;
		}
	}

	return NewBP;
}



#endif






#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDTMultiWindowEditorModule, DTMultiWindowEditor)