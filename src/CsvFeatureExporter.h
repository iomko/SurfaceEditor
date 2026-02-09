#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "Scene/Mesh.h"
//#include "Structures/ExtendedHalfEdge.h"
#include "FeaturesExtractor.h"
#include <filesystem>

class CsvFeatureExporter {
public:
    CsvFeatureExporter(){}

    void appendToCsv(Mesh* mesh) {
            
        std::string fileName = "/home/samuel/Downloads/dataset/data.csv";
        bool fileExists = std::filesystem::exists(fileName);

        std::ofstream file(fileName, std::ios::app);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << fileName << " for appending" << std::endl;
            return;
        }

        if (!fileExists) {
            file << "le1,le2,le3,rt,ang1,ang2,ang3,are,he1,he2,he3,label\n";
        }

        ExtendedHalfEdgeMesh* hem = mesh->getHalfEdgeStructure();
        std::vector<ExtendedFace*>& faces = hem->m_faces;

        for (ExtendedFace* face : faces) {

            auto le = FeaturesExtractor::sideLengths2(face);
            float rt = FeaturesExtractor::ratio2(face);
            auto ang = FeaturesExtractor::internalAngles2(face);
            float are = FeaturesExtractor::area2(face);
            auto he = FeaturesExtractor::heights2(face);
            int label = FeaturesExtractor::isSkewed2(face) ? 1 : 0;

            file << le[0] << ","
                 << le[1] << ","
                 << le[2] << ","
                 << rt << ","
                 << ang[0] << ","
                 << ang[1] << ","
                 << ang[2] << ","
                 << are << ","
                 << he[0] << ","
                 << he[1] << ","
                 << he[2] << ","
                 << label
                 << "\n";
        }
    }

};
