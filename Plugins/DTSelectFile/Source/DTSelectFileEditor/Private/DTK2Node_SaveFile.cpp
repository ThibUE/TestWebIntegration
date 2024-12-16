﻿// Copyright 2022 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "DTK2Node_SaveFile.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "DTSelectFileBPLib.h"
#include "K2Node_CallFunction.h"
#include "K2Node_MakeMap.h"
#include "KismetCompiler.h"
#include "Kismet2/BlueprintEditorUtils.h"

// 构造函数
UDTK2Node_SaveFile::UDTK2Node_SaveFile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

// 菜单注册
void UDTK2Node_SaveFile::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();

	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

// 编译节点
void UDTK2Node_SaveFile::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* ThenPin = GetThenPin();
	if (ExecPin && ThenPin)
	{
		// 创建函数回调
		FName FunctionName = GET_FUNCTION_NAME_CHECKED(UDTSelectFileBPLib, SaveFileDialog);
		UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallFuncNode->FunctionReference.SetExternalMember(FunctionName, UDTSelectFileBPLib::StaticClass());
		CallFuncNode->AllocateDefaultPins();
		
		// 绑定参数节点
		CompilerContext.MovePinLinksToIntermediate(*(FindPinChecked(TEXT("DialogTitle"), EGPD_Input)), *(CallFuncNode->FindPinChecked(TEXT("DialogTitle"))));
		CompilerContext.MovePinLinksToIntermediate(*(FindPinChecked(TEXT("DefaultPath"), EGPD_Input)), *(CallFuncNode->FindPinChecked(TEXT("DefaultPath"))));
		CompilerContext.MovePinLinksToIntermediate(*(FindPinChecked(TEXT("FilePath"), EGPD_Output)), *(CallFuncNode->FindPinChecked(TEXT("FilePath"))));

		// 绑定输入Map
		UK2Node_MakeMap* MakeMapNode = CompilerContext.SpawnIntermediateNode<UK2Node_MakeMap>(this, SourceGraph);
		MakeMapNode->AllocateDefaultPins();

		// 绑定到变量
		UEdGraphPin* MapOut = MakeMapNode->GetOutputPin();
		UEdGraphPin* FuncArgPin = CallFuncNode->FindPinChecked(TEXT("FileType"));
		MapOut->MakeLinkTo(FuncArgPin);
		MakeMapNode->PinConnectionListChanged(MapOut);

		// 绑定到输入值
		int nIndex = 0;
		for (const FTypeStruct & TypeStruct : m_ArrayType)
		{
			// 新增默认点
			if (nIndex > 0)
				MakeMapNode->AddInputPin();

			// 设置点
			UEdGraphPin* pShowInfo = FindPinChecked(TypeStruct.szShowInfoName);
			UEdGraphPin* pKeyPin = MakeMapNode->FindPinChecked(*FString::Printf(TEXT("Key %d"), nIndex));
			CompilerContext.MovePinLinksToIntermediate(*pShowInfo, *pKeyPin);
			
			UEdGraphPin* pFileType = FindPinChecked(TypeStruct.szFileTypeName);
			UEdGraphPin* ValuePin = MakeMapNode->FindPinChecked(*FString::Printf(TEXT("Value %d"), nIndex));
			CompilerContext.MovePinLinksToIntermediate(*pFileType, *ValuePin);

			// 索引增加
			nIndex++;
		}
		
		// 绑定执行节点
		CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(CallFuncNode->GetExecPin()));
		CompilerContext.MovePinLinksToIntermediate(*ThenPin, *(CallFuncNode->GetThenPin()));
	}

	BreakAllNodeLinks();
}