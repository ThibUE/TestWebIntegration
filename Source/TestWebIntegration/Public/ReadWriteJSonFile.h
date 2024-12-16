// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadWriteJSonFile.generated.h"

class FJsonObject;

USTRUCT(BlueprintType)
struct FVisitInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString VisitName = "Default";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString SavePath = "../../../../../../CNDH_Projet/TestDataLoading/Saved/SaveGames/Save1.sav";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int Index = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Langue = "Francais";
};

USTRUCT(BlueprintType)
struct FAllVisitsInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FVisitInfo> AllVisits;
};

UCLASS()
class TESTWEBINTEGRATION_API UReadWriteJSonFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static FAllVisitsInfo ReadInfosFromJsonFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage);
		
	UFUNCTION(BlueprintCallable)
		static void WriteInfosToJsonFile(FString FilePath, FAllVisitsInfo Infos, bool& bOutSuccess, FString& OutInfoMessage);

public:
	static TSharedPtr<FJsonObject> ReadJson(FString JSonFilePath, bool& bOutSuccess, FString& OutInfoMessage);
	static void WriteJSon(FString FilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSuccess, FString& OutInfoMessage);

};
