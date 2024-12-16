// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DTUserWidget.generated.h"

/**
 * The extended Widget of UserWidget can be used for system window pop-up display.
 */
UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta=( DontUseGenericSpawnObject="True", DisableNativeTick, DisplayName="DT User Widget") )
class DTMULTIWINDOW_API UDTUserWidget : public UUserWidget
{
	GENERATED_BODY()

	// 变量定义
protected:
	TWeakPtr<SWindow>					m_MainWindow;			// 绑定的主窗口

public:
	// 设置绑定的主窗口
	void SetMainWindow( const TSharedPtr<SWindow> & MainWindow );
	
public:
	// Window is activated
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic, Category = "User Interface|DT User Widget")
	void OnWindowActivated();
	void OnWindowActivated_Implementation() {};

	// Window is deactivated.
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic, Category = "User Interface|DT User Widget")
	void OnWindowDeactivated();
	void OnWindowDeactivated_Implementation() {};

	// Window is closed.
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic, Category = "User Interface|DT User Widget")
	void OnWindowClosed();
	void OnWindowClosed_Implementation() {};

	// Show window
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "User Interface|DT User Widget")
	void ShowWindow();

	// Hide window
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "User Interface|DT User Widget")
	void HideWindow();

	// Close window
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "User Interface|DT User Widget")
	void CloseWindow();

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "User Interface|DT User Widget")
    FVector2D GetPositionInScreen() const;
    	
	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "User Interface|DT User Widget")
	FVector2D GetSizeInScreen() const;
};
