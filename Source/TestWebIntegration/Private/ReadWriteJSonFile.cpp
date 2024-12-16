// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadWriteJSonFile.h"
#include "ReadWriteFile.h"

#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

void UReadWriteJSonFile::WriteJSon(FString FilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSuccess, FString& OutInfoMessage)
{
	FString JsonString;
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString,0)))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write Json failed - Not able to serialize the Json to string. Is Json object valid ? - '%s'"), *JsonString);
		return ;
	}

	UReadWriteFile::WriteStringToFile(FilePath, JsonString, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess) return;

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write Json file succeded - '%s'"), *FilePath);
	return;
}

FAllVisitsInfo UReadWriteJSonFile::ReadInfosFromJsonFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	TSharedPtr<FJsonObject> JSonObject = ReadJson(FilePath, bOutSuccess, OutInfoMessage);
	if(!bOutSuccess) 
		return FAllVisitsInfo();

	FAllVisitsInfo RetAllVisitsInfo;
	if (!FJsonObjectConverter::JsonObjectToUStruct<FAllVisitsInfo>(JSonObject.ToSharedRef(), &RetAllVisitsInfo))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read Struct Json failed - Not able to convert the json object to your desired structure. Was righ format / struct ? - '%s'"));
		return FAllVisitsInfo();
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read Struct Json succeded - '%s'"), *FilePath);
	return RetAllVisitsInfo;
}

void UReadWriteJSonFile::WriteInfosToJsonFile(FString FilePath, FAllVisitsInfo Infos, bool& bOutSuccess, FString& OutInfoMessage)
{
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Infos);
	if (JsonObject == nullptr)
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write struct Json failed - Not able to convert the string to a json Object."));
		return ;
	}

	WriteJSon(FilePath, JsonObject, bOutSuccess, OutInfoMessage);
}


TSharedPtr<FJsonObject> UReadWriteJSonFile::ReadJson(FString JSonFilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	FString JsonString = UReadWriteFile::ReadStringFromFile(JSonFilePath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
		return nullptr;

	TSharedPtr<FJsonObject> RetJsonObject;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), RetJsonObject))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read Json failed - Not able to deserialize the Json string. Was righ format ? - '%s'"), *JsonString);
		return nullptr;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read Json file succeded - '%s'"), *JSonFilePath);
	return RetJsonObject;
}
