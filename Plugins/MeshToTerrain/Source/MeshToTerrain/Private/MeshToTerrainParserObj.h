#pragma once

#include "MeshToTerrainPrivatePCH.h"
#include "MeshToTerrainModel.h"
#include "IMeshToTerrainParser.h"

/* Basic OBJ file parser*/
class FMeshToTerrainParserObj : public IMeshToTerrainParser 
{

  private:
	  FVector MeshSize;
	  const TCHAR * Filename;

  public:
	  FMeshToTerrainParserObj(const TCHAR* ObjFilename);
	   
      /* IMeshToTerrainParser interface methods */
	  virtual int   ParseFile(FVector & BoundingBoxSize) const override;
	  virtual int   ParseFile(FMeshToTerrainModel & Model) const override;

  private:
	  int ParseComment(const FString & Line) const;
	  int ParseVertex(const FString & Line, FVector & Vertex) const;
	  int ParseVertexNormal(const FString & Line) const;
	  int ParseVertexParameter(const FString & Line) const;
	  int ParseFace(const FString & Line, TArray<int> & Vertexes) const;
};

