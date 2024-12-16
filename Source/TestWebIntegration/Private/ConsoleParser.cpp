// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsoleParser.h"

// Sets default values
AConsoleParser::AConsoleParser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	position = FVector(-430, 720, 142);

	positionMatches.Add("Entree exterieure", FVector(2412.981832, 3434.288207, 512.771274));
	positionMatches.Add("Entree interieure", FVector(370,700, 142));
	positionMatches.Add("Le salon de the 1", FVector(770,930, 142));
	positionMatches.Add("Le salon de the 2", FVector(1300,900,142));
	positionMatches.Add("Espace epicerie", FVector(1815,565,142));
	positionMatches.Add("Espace figurines", FVector(3567.952010, 4839.786478, 2709.577943));
}

FVector AConsoleParser::getPosition()
{
	return position;
}

FVector AConsoleParser::getRotation()
{
	return rotation;
}

void AConsoleParser::UpdatePosition(FString pos)
{
	if (!pos.Contains("POS_UPDATE"))
		return ;

	FString positionName;
	pos.Split(":",NULL,&positionName);
	positionName = positionName.TrimStart().TrimEnd();

	if (positionMatches.Contains(positionName))
	{
		position = *positionMatches.Find(positionName);
	}	
}

void AConsoleParser::UpdateRotation(FString rot)
{
	if (!rot.Contains("ROT_UPDATE"))
		return;

	FString Rotation;
	rot.Split(":", NULL, &Rotation);
	Rotation = Rotation.TrimStart().TrimEnd();

	FString DirText;
	FString EleText;
	Rotation.Split(" ", &DirText, &EleText);

	FString Dir;
	DirText.Split(":", NULL, &Dir);

	FString Ele;
	EleText.Split(":", NULL, &Ele);

	float dirValue = FCString::Atof(*Dir);
	if (dirValue < 0)
	{
		dirValue = 12 + dirValue;
	}
	dirValue = dirValue - 6;

	float dirMapped =  (dirValue + 6) * (360) / (12);
	if (dirMapped >= 180) dirMapped = dirMapped - 360;

	float eleValue = FCString::Atof(*Ele);
	float eleMapped = -90 + (eleValue + 3) * (180) / (6);

	rotation = FVector(0,eleMapped,dirMapped);


}

int AConsoleParser::GetPhotoPositionIndex(AActor* objectReference)
{
	UKismetSystemLibrary::PrintString(GetWorld(), objectReference->GetActorNameOrLabel(), true, true, FLinearColor::Red, 5.f);
	return PhotoPositionsActors.IndexOfByKey(objectReference);
}

// Called when the game starts or when spawned
void AConsoleParser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConsoleParser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

