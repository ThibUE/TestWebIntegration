#include "MeshToTerrainParserFactory.h"
#include "MeshToTerrainParserObj.h"
#include "MeshToTerrainParserFBX.h"

TSharedPtr<IMeshToTerrainParser> FMeshToTerrainParserFactory::CreateParser(const TCHAR * Filename)
{
	FString File = FString(Filename);
	
    if (File.EndsWith(".fbx"))
    {
        return MakeShareable(new FMeshToTerrainParserFBX(Filename));
    }

    // REVISIT: consider parsing the obj files with the FBX parser as it should
    // be more complete
    if (File.EndsWith(".obj"))
    {
        return MakeShareable(new FMeshToTerrainParserObj(Filename));
    }

	return nullptr;
}
