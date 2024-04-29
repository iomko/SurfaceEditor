#pragma once
#include <iostream>

#include "SelectionCallBacks/FacesDataTransfer.h"
#include "Callback.h"
#include "../ViewPortHolder.h"

class VertexModeChangeCallBack : public Callback
{
public:
    VertexModeChangeCallBack(ViewPortHolder* viewPortHolder)
    {
        m_viewPortHolder = viewPortHolder;
    }

    void execute() override
    {
        //musime pridat do selectedFaces vsetky faces ktore su vo FacesDataTransfer
        if (FacesDataTransfer::m_transferedData.size() > 0)
        {
            //tak musime pridat tieto faces do selectedFaces


            for (const auto& transferedFacesDataMap : FacesDataTransfer::m_transferedData) {
                Mesh* currentMesh = transferedFacesDataMap.first;
                const std::set<HalfEdgeDS::Face*>& currentFacesSet = transferedFacesDataMap.second;


                for (HalfEdgeDS::Face* transferedFace : currentFacesSet) {
                    m_viewPortHolder->m_selectedFaces[currentMesh].insert(transferedFace);

                    //no takto sme ju ale pridali len do vectora ktory nie je este urceny na rendering

                    //preto ju musime taktiez pridat do vectora na rendering
                    //a tym je m_combinedVector a materialVectors


                    HalfEdgeDS::FaceIndex faceIndex = transferedFace->getHalfEdge()->getFaceIndex();
                    size_t faceStartIndex = currentMesh->m_facesData.at(faceIndex).startIndex;
                    size_t faceEndIndex = currentMesh->m_facesData.at(faceIndex).endIndex;

                    //ist do m_combinedVectora
                    for (int i = faceStartIndex; i <= faceEndIndex; ++i)
                    {
                        //m_combinedVector
                        currentMesh->m_combinedVertexDataMatVector.m_vertexData.at(i).isHighlited = 1.0f;
                        //material vectors
                        Material* materialInMaterialVector = currentMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).material;
                        size_t indexInMaterialVector = currentMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).index;
                        auto materialVectorIt = currentMesh->m_materialIDVertexDataMap.find(materialInMaterialVector);
                        materialVectorIt->second.m_vertexData.at(indexInMaterialVector).isHighlited = 1.0f;
                    }


                }
            }

            //na konci vymazat transferedVector

            FacesDataTransfer::m_transferedData.clear();

        }
    }


private:
    ViewPortHolder* m_viewPortHolder;
};
