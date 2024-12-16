// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#pragma once

#include "DTUserWidget.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widgets/SWindow.h"
#include "DTMultiWindowBPLib.generated.h"

/** Enumeration to specify different window types for SWindows */
UENUM(BlueprintType)
enum class EDTWindowType : uint8
{
	/** Value indicating that this is a standard, general-purpose window */
	Normal,
	/** Value indicating that this is a generic window without borders */
	NoBorder,
};

/** Enum to describe how to auto-center an SWindow */
UENUM(BlueprintType)
enum class EDTAutoCenter : uint8
{
	/** Don't auto-center the window */
	None,

	/** Auto-center the window on the primary work area */
	PrimaryWorkArea,

	/** Auto-center the window on the preferred work area, determined using GetPreferredWorkArea() */
	PreferredWorkArea,
};

/** Enum to describe how windows are sized */
UENUM(BlueprintType)
enum class EDTSizingRule : uint8
{
	/* The windows size fixed and cannot be resized **/
	FixedSize,

	/** The window size is computed from its content and cannot be resized by users */
	Autosized,

	/** The window can be resized by users */
	UserSized,
};


UCLASS(NotBlueprintable, NotBlueprintType, meta=(DisplayName="DT Multi Window"))
class DTMULTIWINDOW_API UDTMultiWindowBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	// 转换类型
	static EWindowType ConvertWindowType(EDTWindowType WindowType);
	static EAutoCenter ConvertAutoCenter(EDTAutoCenter WindowType);
	static ESizingRule ConvertSizingRule(EDTSizingRule SizingRule);

public:
	// Close and destroy all the currently created windows
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "User Interface|DT Multi Window")
	static void CloseAllWindow();
	
	/**
	 *	Window is a platform-agnostic representation of a top-level window.
	 *	@param WorldContextObject The current world.
	 *	@param Class The type of DT User Widget.
	 *	@param Title Title of the window.
	 *	@param ClientSize What the initial size of the window should be.
	 *	@param DoModal Modal window.
	 *	@param Show Show window.
	 *	@param WindowType Type of this window.
	 *	@param ScreenPosition Screen-space position where the window should be initially located.
	 *	@param AutoCenter The windows auto-centering mode. If set to anything other than None, then the ScreenPosition value will be ignored.
	 *	@param DragAnywhere The windows auto-ceWhen true, the window can be dragged from anywhere.
	 *	@param IsInitiallyMaximized Is the window initially maximized.
	 *	@param IsInitiallyMinimized Is the window initially minimized.
	 *	@param SizingRule How the window should be sized.
	 *	@param IsTopmostWindow True if this window should always be on top of all other windows.
	 *	@param FocusWhenFirstShown Should this window be focused immediately after it is shown?
	 *	@param HasCloseButton Does this window have a close button?
	 *	@param SupportsMaximize Can this window be maximized?
	 *	@param SupportsMinimize Can this window be minimized?
	 *	@param ShouldPreserveAspectRatio Should this window preserve its aspect ratio when resized by user? 
	 *	@param MinWidth The smallest width this window can be in Desktop Pixel Units.
	 *	@param MinHeight The smallest height this window can be in Desktop Pixel Units.
	 *	@param MaxWidth The biggest width this window can be in Desktop Pixel Units.
	 *	@param MaxHeight The biggest height this window can be in Desktop Pixel Units.
	 */
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly="true", UnsafeDuringActorConstruction = "true", WorldContext="WorldContextObject"))
	static UDTUserWidget * CreateWindow(
		UObject* WorldContextObject,
		TSubclassOf<UDTUserWidget> Class,
		FText Title,
		FVector2D ClientSize,
		bool DoModal = false,
		bool Show = true,
		EDTWindowType WindowType = EDTWindowType::Normal,
		FVector2D ScreenPosition = FVector2D::ZeroVector,
		EDTAutoCenter AutoCenter = EDTAutoCenter::PreferredWorkArea,
		bool DragAnywhere = false,
		bool IsInitiallyMaximized = false,
		bool IsInitiallyMinimized = false,
		EDTSizingRule SizingRule = EDTSizingRule::FixedSize,
		bool IsTopmostWindow = false,
		bool FocusWhenFirstShown = true,
		bool HasCloseButton = true,
		bool SupportsMaximize = true,
		bool SupportsMinimize = true,
		bool ShouldPreserveAspectRatio = false,
		float MinWidth = 0.f,
		float MinHeight = 0.f,
		float MaxWidth = 0.f,
		float MaxHeight = 0.f);
};
