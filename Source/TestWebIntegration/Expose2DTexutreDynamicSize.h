// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Texture2DDynamic.h"
#include "Expose2DTexutreDynamicSize.generated.h"


/**
 * 
 */
UCLASS()
class TESTWEBINTEGRATION_API UExpose2DTexutreDynamicSize : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
		static int32 GetTexture2DDynamicWidth(UTexture2DDynamic* InTexture2DDynamicReference);

	UFUNCTION(BlueprintCallable)
		static int32 GetTexture2DDynamicHeight(UTexture2DDynamic* InTexture2DDynamicReference);
};
