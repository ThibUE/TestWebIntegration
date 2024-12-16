#pragma once

#include "MeshToTerrainPrivatePCH.h"

#include "IMeshToTerrainModel.h"


class FMeshToTerrainModel : public IMeshToTerrainModel
{
	private:
		TArray<FMeshToTerrainModelVertex> Vertex;
		TArray<FMeshToTerrainModelFace>   Face;

	public:
		FMeshToTerrainModel();
        virtual ~FMeshToTerrainModel();
		
		/* IMeshToTerrainModel interface methods */
		virtual void AddShape(FMeshToTerrainModelVertex Vrtx);
		virtual void AddShape(FMeshToTerrainModelFace   Fce);
		
		/* Converts the internal representation into a heightmap of size MapSize*/
		void ConvertData(TArray<uint16> &Data, const FIntPoint & MapSize);

	private:
		/* Computes the bounding box of the internal data*/
		FBox ComputeBoundingBox();

        /* Performs various preprocessing of the model data to ease any required computation */
        void PreProprocessModelData();
};
