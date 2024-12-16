// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#include "DTUserWidget.h"
#include "Widgets/SWidget.h"

void UDTUserWidget::SetMainWindow( const TSharedPtr<SWindow> & MainWindow)
{
	m_MainWindow = MainWindow;
}

void UDTUserWidget::ShowWindow()
{
	if ( const TSharedPtr<SWindow> Window = m_MainWindow.Pin() )
	{
		Window->ShowWindow();
	}
}

void UDTUserWidget::HideWindow()
{
	if ( const TSharedPtr<SWindow> Window = m_MainWindow.Pin() )
	{
		Window->HideWindow();
	}
}

void UDTUserWidget::CloseWindow()
{
	if ( const TSharedPtr<SWindow> Window = m_MainWindow.Pin() )
	{
		Window->HideWindow();
		Window->RequestDestroyWindow();
	}
}

FVector2D UDTUserWidget::GetPositionInScreen() const
{
	if ( const TSharedPtr<SWindow> Window = m_MainWindow.Pin() )
	{
		return Window->GetPositionInScreen();
	}
	
	return FVector2D::ZeroVector;
}

FVector2D UDTUserWidget::GetSizeInScreen() const
{
	if ( const TSharedPtr<SWindow> Window = m_MainWindow.Pin() )
	{
		return Window->GetSizeInScreen();
	}
	
	return FVector2D::ZeroVector;
}
