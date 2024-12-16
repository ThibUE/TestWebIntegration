// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "DTMultiWindowBPLib.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SWindow.h"
#include "Widgets/SViewport.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Docking/TabManager.h"

TArray< TSharedRef<SWindow> > g_ArrayWindows;

EWindowType UDTMultiWindowBPLib::ConvertWindowType(EDTWindowType WindowType)
{
	// 窗口类型
	switch (WindowType)
	{
	case EDTWindowType::Normal:
		return EWindowType::Normal;
	case EDTWindowType::NoBorder:
		return EWindowType::Menu;
	default:
		return EWindowType::Normal;
	}
}

EAutoCenter UDTMultiWindowBPLib::ConvertAutoCenter(EDTAutoCenter AutoCenter)
{
	// 居中类型类型
	switch (AutoCenter)
	{
	case EDTAutoCenter::None:
		return EAutoCenter::None;
	case EDTAutoCenter::PrimaryWorkArea:
		return EAutoCenter::PrimaryWorkArea;
	case EDTAutoCenter::PreferredWorkArea:
		return EAutoCenter::PreferredWorkArea;
	default:
		return EAutoCenter::PreferredWorkArea;
	}
}

ESizingRule UDTMultiWindowBPLib::ConvertSizingRule(EDTSizingRule SizingRule)
{
	switch (SizingRule)
	{
	case EDTSizingRule::FixedSize:
		return ESizingRule::FixedSize;
	case EDTSizingRule::Autosized:
		return ESizingRule::Autosized;
	case EDTSizingRule::UserSized:
		return ESizingRule::UserSized;
	default:
		return ESizingRule::FixedSize;
	}
}

void UDTMultiWindowBPLib::CloseAllWindow()
{
	for (auto ItrWindow = g_ArrayWindows.CreateIterator(); ItrWindow; ++ItrWindow)
	{
		SWindow * Window = &((*ItrWindow).Get());
		ItrWindow.RemoveCurrent();
		Window->RequestDestroyWindow();
	}
}

UDTUserWidget * UDTMultiWindowBPLib::CreateWindow(
	UObject* WorldContextObject,
	TSubclassOf<UDTUserWidget> Class,
	FText Title,
	FVector2D ClientSize,
	bool DoModal,
	bool Show,
	EDTWindowType WindowType,
	FVector2D ScreenPosition,
	EDTAutoCenter AutoCenter,
	bool DragAnywhere,
	bool IsInitiallyMaximized,
	bool IsInitiallyMinimized,
	EDTSizingRule SizingRule,
	bool IsTopmostWindow,
	bool FocusWhenFirstShown,
	bool HasCloseButton,
	bool SupportsMaximize,
	bool SupportsMinimize,
	bool ShouldPreserveAspectRatio,
	float MinWidth,
	float MinHeight,
	float MaxWidth,
	float MaxHeight )
{
	// 获取世界指针
	UWorld* pWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	
	// 创建UMG
	UDTUserWidget * DTUserWidget = CreateWidget<UDTUserWidget>(pWorld, Class);
	TSharedRef<SWidget> Widget = DTUserWidget->TakeWidget();
	
	// 新窗口风格
	const TSharedRef<SWindow> NewWindow = SNew(SWindow)
	.Title(Title)
	.ClientSize(ClientSize)
	.ScreenPosition(ScreenPosition)
	.Type( ConvertWindowType(WindowType) )
	.AutoCenter( ConvertAutoCenter(AutoCenter) )
	.bDragAnywhere(DragAnywhere)
	.IsInitiallyMaximized(IsInitiallyMaximized)
	.IsInitiallyMinimized(IsInitiallyMinimized)
	.SizingRule(ConvertSizingRule(SizingRule))
	.HasCloseButton(HasCloseButton)
	.SupportsMaximize(SupportsMaximize)
	.SupportsMinimize(SupportsMinimize)
	.ShouldPreserveAspectRatio(ShouldPreserveAspectRatio)
	.MinWidth( ((MinWidth < 1.f) ? TOptional<float>(0.f) : TOptional<float>(MinWidth)) )
	.MinHeight( ((MinHeight < 1.f) ? TOptional<float>(0.f) : TOptional<float>(MinHeight)) )
	.MaxWidth( ((MaxWidth < 1.f) ? TOptional<float>() : TOptional<float>(MaxWidth)) )
	.MaxHeight( ((MaxHeight < 1.f) ? TOptional<float>() : TOptional<float>(MaxHeight)) )
	.IsTopmostWindow(IsTopmostWindow)
	.FocusWhenFirstShown(FocusWhenFirstShown)
	.UseOSWindowBorder(WindowType == EDTWindowType::Normal)
	[
		Widget
	];

	// 设置激活绑定控件
	NewWindow->SetWidgetToFocusOnActivate(Widget);
	
	// 窗口激活
	NewWindow->GetOnWindowActivatedEvent().AddLambda([DTUserWidget]()
	{
		DTUserWidget->OnWindowActivated();
	});
	
	// 窗口停用
	NewWindow->GetOnWindowDeactivatedEvent().AddLambda([DTUserWidget]()
	{
		DTUserWidget->OnWindowDeactivated();
	});
	
	// 关闭消息回调
	NewWindow->GetOnWindowClosedEvent().AddLambda([DTUserWidget](const TSharedRef<SWindow>& Window)
	{
		DTUserWidget->OnWindowClosed();
		g_ArrayWindows.Remove(Window);
	});

	// 设置UMG主窗口
	DTUserWidget->SetMainWindow(NewWindow);
	g_ArrayWindows.Add(NewWindow);
	
	// 添加窗口显示
	if ( DoModal )
	{
		// 模态窗口
		FSlateApplication::Get().AddModalWindow(NewWindow,FGlobalTabmanager::Get()->GetRootWindow());
		
		// 还原到主窗口焦点
		const TSharedPtr<SViewport> ViewportWidget = GEngine->GameViewport->GetGameViewportWidget();
		FSlateApplication::Get().SetKeyboardFocus(ViewportWidget);
		FSlateApplication::Get().SetAllUserFocus(ViewportWidget);
	}
	else
	{
		FSlateApplication::Get().AddWindow(NewWindow, Show);
	}

	return DTUserWidget;
}
