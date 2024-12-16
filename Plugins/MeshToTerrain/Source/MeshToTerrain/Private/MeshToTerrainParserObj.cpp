
#include "MeshToTerrainParserObj.h"
#include "Runtime/Core/Public/Internationalization/Regex.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"

FMeshToTerrainParserObj::FMeshToTerrainParserObj(const TCHAR * ObjFilename)
{
	Filename = ObjFilename;
}

int FMeshToTerrainParserObj::ParseFile(FVector & BoundingBoxSize) const
{
	TArray<FString> Lines;
    if (!FFileHelper::LoadANSITextFileToStrings(Filename, &IFileManager::Get(), Lines))
    {
        return 0;
    }

	FVector Min, Max;
	bool bInitialized = false;

    for (auto& Str : Lines)
    {
        FVector CurrentVertex;
        TArray<int> FaceVertex;

        // Basic sanity checks
        Str.TrimStartInline();
        if (Str.Len() <= 2)
        {
            continue;
        }

        if (!FCString::IsPureAnsi(*Str))
        {
            return 0;
        }

        // Comment 
        if (ParseComment(Str))
        {
            continue;
        }
        // In order to find the bounding box of the mesh we only 
        // care about the vertices
        if (ParseVertex(Str, CurrentVertex))
        {
            if (!bInitialized)
            {
                Min = CurrentVertex;
                Max = CurrentVertex;
                bInitialized = true;
            }
            if (CurrentVertex.X < Min.X) Min.X = CurrentVertex.X;
            if (CurrentVertex.Y < Min.Y) Min.Y = CurrentVertex.Y;
            if (CurrentVertex.Z < Min.Z) Min.Z = CurrentVertex.Z;

            if (CurrentVertex.X > Max.X) Max.X = CurrentVertex.X;
            if (CurrentVertex.Y > Max.Y) Max.Y = CurrentVertex.Y;
            if (CurrentVertex.Z > Max.Z) Max.Z = CurrentVertex.Z;

            continue;
        }
        if (ParseVertexNormal(Str))
        {
            continue;
        }
        if (ParseVertexParameter(Str))
        {
            continue;
        }
        if (ParseFace(Str, FaceVertex))
        {
            continue;
        }
	}

	BoundingBoxSize = Max - Min;
	return 1;
}

int FMeshToTerrainParserObj::ParseFile(FMeshToTerrainModel & Model) const
{
	TArray<FString> Lines;
    if (!FFileHelper::LoadANSITextFileToStrings(Filename, &IFileManager::Get(), Lines))
    {
        return 0;
    }

    for (auto& Str : Lines)
    {
        FVector CurrentVertex;
        TArray<int> FaceVertex;

        // Basic sanity checks
        Str.TrimStartInline();
        if (Str.Len() <= 2)
        {
            continue;
        }

        if (!FCString::IsPureAnsi(*Str))
        {
            return 0;
        }

        if (ParseComment(Str))
        {
            continue;
        }
        if (ParseVertex(Str, CurrentVertex))
        {
            Model.AddShape(CurrentVertex);
            continue;
        }
        if (ParseVertexNormal(Str))
        {
            continue;
        }
        if (ParseVertexParameter(Str))
        {
            continue;
        }
		if (ParseFace(Str, FaceVertex))
		{
			Model.AddShape(FaceVertex);
			continue;
		}
	}

	return 1;
}

int FMeshToTerrainParserObj::ParseComment(const FString & Line) const
{
	return 0;
}

int FMeshToTerrainParserObj::ParseVertex(const FString & Line, FVector & Vertex) const
{
	Vertex.X = 0;
	Vertex.Y = 0;
	Vertex.Z = 0;

    // See the OBJ format spec ref. vertex lines
	if ((Line[0] == 'v') && (Line[1] == ' '))
	{
		TArray<FString> Tokens;
		Line.ParseIntoArray(Tokens, TEXT(" "), true);

        if (Tokens.Num() < 4)
        {
            return 0;
        }

		Vertex.X = FCString::Atof(*Tokens[1]);
		Vertex.Y = FCString::Atof(*Tokens[2]);
		Vertex.Z = FCString::Atof(*Tokens[3]);

		return 1;
	}
	return 0;
}

int FMeshToTerrainParserObj::ParseVertexNormal(const FString & Line) const
{
	return 0;
}

int FMeshToTerrainParserObj::ParseVertexParameter(const FString & Line) const
{
	return 0;
}

int FMeshToTerrainParserObj::ParseFace(const FString & Line, TArray<int> & Vertexes) const
{
    // See the OBJ format spec ref. face lines
	if ((Line[0] == 'f') && (Line[1] == ' '))
	{
		TArray<FString> Tokens;
		Line.ParseIntoArray(Tokens, TEXT(" "), true);

        if (Tokens.Num() < 4)
        {
            return 0;
        }

		for (int i = 1; i<Tokens.Num(); i++)
		{
			const TCHAR * CurrentCh  = *Tokens[i];
			int   Sign               = 1;
			int   Index              = 0;

			// Negative offset or end of string
			if (*CurrentCh == '-') 
			{
				Sign = -1;
				CurrentCh++;
			}
            else if (*CurrentCh == 0)
            {
                continue;
            }

			// Extract number
			while (FChar::IsDigit(*CurrentCh))
			{
				Index = Index * 10 + FChar::ConvertCharDigitToInt(*CurrentCh);
				CurrentCh++;
			}

			Index *= Sign;
			Vertexes.Add(Index);
		}
		return 1;
	}
	return 0;
}