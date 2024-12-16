// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadWriteFile.h"

#include "HAL/PlatformFileManager.h"
#include "misc/FileHelper.h"

FString UReadWriteFile::ReadStringFromFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read string from file failed - File does not exist - '%s'"), *FilePath);
		return "";
	}

	FString RetString = "";

	if (!FFileHelper::LoadFileToString(RetString, *FilePath))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read string from file failed - Not able to read file. Is this a text file ? - '%s'"), *FilePath);
		return "";
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read string from file succeded - '%s'"), *FilePath);
	return RetString;
}

void UReadWriteFile::WriteStringToFile(FString FilePath, FString String, bool& bOutSuccess, FString& OutInfoMessage)
{
	if (!FFileHelper::SaveStringToFile(String, *FilePath))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write string to file failed - Not able to write file. File in read only ? Path valid ? - '%s'"), *FilePath);
		return ;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write string to file succeded - '%s'"), *FilePath);
	return ;
}
