// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "IAssetTools.h"
#include "IAssetTypeActions.h"
#include "Modules/ModuleManager.h"
#include "DTMultiWindowEditor.generated.h"

class FDTMultiWindowEditorModule : public IModuleInterface
{
public:
    // 系统开始运行
    virtual void StartupModule() override;
    // 系统结束运行
    virtual void ShutdownModule() override;

public:
    // PIE开始
    void OnPreBeginPIE(const bool bIsSimulatingInEditor);
    // PIE结束
    void OnEndPIE(const bool bIsSimulatingInEditor);

#if WITH_EDITOR

private:
    TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;
    static EAssetTypeCategories::Type SubsystemAssetCategory;
	
public:
    // 返回分类
    static EAssetTypeCategories::Type GetAssetCategory() { return SubsystemAssetCategory; }
    // 添加动作
    void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

#endif
};

#if WITH_EDITOR

UCLASS()
class DTMULTIWINDOWEDITOR_API UDTUserWidgetAction_Factory : public UFactory
{
    GENERATED_UCLASS_BODY()
public:
    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

#endif
