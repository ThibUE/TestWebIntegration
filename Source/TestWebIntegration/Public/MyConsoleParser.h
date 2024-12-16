// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <string>
#include "MyConsoleParser.generated.h"

/**
 * 
 */
UCLASS()
class TESTWEBINTEGRATION_API UMyConsoleParser : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//UFUNCTION(BlueprintPure, Category = "Test")
	//bool test() const;

	UFUNCTION(BlueprintCallable, Category = "Test")
		void UpdatePosition(bool pos);

};
