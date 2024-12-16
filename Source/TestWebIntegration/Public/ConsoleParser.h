// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
#include "Containers/Map.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "ConsoleParser.generated.h"


UCLASS()
class TESTWEBINTEGRATION_API AConsoleParser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConsoleParser();

	// Gets 3D character position
	UFUNCTION(BlueprintPure, Category = "ConsoleParser")
		FVector getPosition();

	UFUNCTION(BlueprintPure, Category = "ConsoleParser")
		FVector getRotation();

	UFUNCTION(BlueprintCallable, Category = "ConsoleParser")
		void UpdatePosition(FString pos);

	UFUNCTION(BlueprintCallable, Category = "ConsoleParser")
		void UpdateRotation(FString rot);

	UFUNCTION(BlueprintCallable, Category = "ConsoleParser")
		int GetPhotoPositionIndex(AActor* objectReference);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<AActor*> PhotoPositionsActors; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector position;
	FVector rotation;
	TMap<FString,FVector> positionMatches;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
