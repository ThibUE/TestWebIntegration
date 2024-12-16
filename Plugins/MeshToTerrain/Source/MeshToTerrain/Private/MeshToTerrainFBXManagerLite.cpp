//#include "MeshToTerrainPrivatePCH.h"
#include "MeshToTerrainFBXManagerLite.h"

FMeshToTerrainFBXManagerLite::FMeshToTerrainFBXManagerLite(const TCHAR * Filename) 
	: Filename(Filename)
{}

FMeshToTerrainFBXManagerLite::~FMeshToTerrainFBXManagerLite()
{
	if (Scene != nullptr)
	{
		Scene->Destroy();
	}
	if (Manager != nullptr)
	{
		Manager->Destroy();
	}
}

bool FMeshToTerrainFBXManagerLite::Init()
{
	Manager = FbxManager::Create();

	// Default settings would do
	FbxIOSettings * Ios = FbxIOSettings::Create(Manager, IOSROOT);
	Manager->SetIOSettings(Ios);
	
	// Try to import the scene
	FbxImporter * Importer = FbxImporter::Create(Manager, "");
	if (!Importer->Initialize((TCHAR_TO_UTF8(Filename)), -1, Manager->GetIOSettings())) {
		return false;
	}
	Scene = FbxScene::Create(Manager, "ConverterMeshObj");
	Importer->Import(Scene);
	Importer->Destroy();
 
	return true;
}

void FMeshToTerrainFBXManagerLite::ExtractMeshVertices(TArray<FVector> & Vrtx)
{
	FbxNode*  Root = Scene->GetRootNode();
	FbxNode * MeshNode = FindFirstMesh(Root);
	bool      bUnrealAxis = true;
	
	// Sanity checks
	if (MeshNode == nullptr)
	{
		return;
	}

	FbxMesh * Mesh = MeshNode->GetMesh();
	if (!Mesh)
	{
		return;
	}
	
	FbxAMatrix& lGlobalTransform = MeshNode->EvaluateGlobalTransform();

	FbxVector4 * ControlPoints    = Mesh->GetControlPoints();
	int NumControlPoints          = Mesh->GetControlPointsCount();

	// Check which coordinate system the mesh uses
	FbxAxisSystem::ECoordSystem CoordSystem = FbxAxisSystem::eRightHanded;
	FbxAxisSystem::EUpVector       UpVector = FbxAxisSystem::eZAxis;
	FbxAxisSystem::EFrontVector FrontVector = (FbxAxisSystem::EFrontVector) - FbxAxisSystem::eParityOdd;
	
	FbxAxisSystem UnrealImportAxis(UpVector, FrontVector, CoordSystem);
	FbxAxisSystem UnrealImportForceXAxis(UpVector, FbxAxisSystem::eParityEven, CoordSystem);

	FbxAxisSystem SourceSetup = Scene->GetGlobalSettings().GetAxisSystem();
	if ((SourceSetup != UnrealImportAxis) && (SourceSetup != UnrealImportForceXAxis))
	{
		bUnrealAxis = false;
	}

	for (int i = 0; i < NumControlPoints; i++)
	{
		FbxVector4 OriginalVertex;
		
		if (bUnrealAxis)
		{
			OriginalVertex = FbxVector4(ControlPoints[i][0],
										ControlPoints[i][1],
										ControlPoints[i][2],
										1);
		}
		else
		{
			// Force w = 1 and force Maya/Max to Unreal axis conversion
			OriginalVertex = FbxVector4(ControlPoints[i][2],
												   -ControlPoints[i][0],
													ControlPoints[i][1],
													1);
		}


// REVISIT: do we actually want to apply the global transform on the vertices?
		FbxVector4 meshPoint = lGlobalTransform.MultT(OriginalVertex);
		Vrtx.Add(FVector(OriginalVertex[0], OriginalVertex[1], OriginalVertex[2]));
////		UE_LOG(MeshToTerrainLog, Warning, TEXT("Row 0: %f %f %f %f"), lGlobalTransform.Get(0,0), lGlobalTransform.Get(0,1), lGlobalTransform.Get(0,2), lGlobalTransform.Get(0,3));
//		UE_LOG(MeshToTerrainLog, Warning, TEXT("Row 1: %f %f %f %f"), lGlobalTransform.Get(1,0), lGlobalTransform.Get(1,1), lGlobalTransform.Get(1,2), lGlobalTransform.Get(1,3));
//		UE_LOG(MeshToTerrainLog, Warning, TEXT("Row 2: %f %f %f %f"), lGlobalTransform.Get(2,0), lGlobalTransform.Get(2,1), lGlobalTransform.Get(2,2), lGlobalTransform.Get(2,3));
//		UE_LOG(MeshToTerrainLog, Warning, TEXT("Row 3: %f %f %f %f"), lGlobalTransform.Get(3,0), lGlobalTransform.Get(3,1), lGlobalTransform.Get(3,2), lGlobalTransform.Get(3,3));
//		UE_LOG(MeshToTerrainLog, Warning, TEXT("Rota : %f %f %f %f"), lGlobalTransform.GetR()[0], lGlobalTransform.GetR()[1], lGlobalTransform.GetR()[2], lGlobalTransform.GetR()[3]);
//		UE_LOG(MeshToTerrainLog, Warning, TEXT("Tran : %f %f %f %f"), lGlobalTransform.GetT()[0], lGlobalTransform.GetT()[1], lGlobalTransform.GetT()[2], lGlobalTransform.GetT()[3]);
//		UE_LOG(MeshToTerrainLog, Warning, TEXT("Scal : %f %f %f %f"), lGlobalTransform.GetS()[0], lGlobalTransform.GetS()[1], lGlobalTransform.GetS()[2], lGlobalTransform.GetS()[3]);

		}
	return;
}

void FMeshToTerrainFBXManagerLite::ExtractMeshFaces(TArray<TArray<int>>& Faces)
{
	FbxNode*  Root = Scene->GetRootNode();
	FbxNode * MeshNode = FindFirstMesh(Root);

	// Sanity checks
	if (MeshNode == nullptr)
	{
		return;
	}

	FbxMesh * Mesh = MeshNode->GetMesh();
	if (!Mesh)
	{
		return;
	}

	int NumFaces = Mesh->GetPolygonCount();
	int *ControlPoints = Mesh->GetPolygonVertices();

	for (int i = 0; i < NumFaces; i++)
	{
		// See the FBX spec for more details on how these are represented
		TArray<int> FaceIndexes;
		int StartIndex      = Mesh->GetPolygonVertexIndex(i);
		int CurrentFaceSize = Mesh->GetPolygonSize(i);
		int *StartAddr      = &(ControlPoints[StartIndex]);

		for (int j = 0; j < CurrentFaceSize; j++)
		{
			FaceIndexes.Add(StartAddr[j]+1);
		}
		Faces.Add(FaceIndexes);
	}

	return;
}

FbxNode * FMeshToTerrainFBXManagerLite::FindFirstMesh(FbxNode * Root)
{
	// Terminal
	if (Root == nullptr)
	{
		return nullptr;
	}

	// Check if the current node is of mesh type
	for (int i = 0; i < Root->GetNodeAttributeCount(); i++)
	{
		FbxNodeAttribute * nodeAttr = Root->GetNodeAttributeByIndex(i);
		if (nodeAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			return Root;
		}
	}

	// Look in a recursive manner if any children is a mesh 
	for (int j = 0; j < Root->GetChildCount(); j++)
	{
		FbxNode * Mesh = FindFirstMesh(Root->GetChild(j));
		if (Mesh != nullptr)
		{
			return Mesh;
		}
	}

	return nullptr;
}
