#pragma once

#include <iostream>
#include <vector>
#include "../Utils/glm_overrides.h"
#include <unordered_set>
#include <map>
#include "../Renderer/Material.h"

using FaceTriangleIndex = int;
using EdgeLineIndex = int;

namespace HalfEdgeDS
{
    template<typename Traits>
	class HalfEdge;
    template<typename Traits>
	class Edge;
    template<typename Traits>
	class Vertex;
    template<typename Traits>
	class Face;
    template<typename Traits>
   	class HalfEdgeMesh;

    struct HalfEdgeTraits {
        using FType = Face<HalfEdgeTraits>;   
        using EType = Edge<HalfEdgeTraits>;
        using HType = HalfEdge<HalfEdgeTraits>;
        using VType = Vertex<HalfEdgeTraits>;
        using DerType = HalfEdgeMesh<HalfEdgeTraits>;
    };

    template<typename Traits = HalfEdgeTraits>
	class HalfEdge
	{
	public:
		HalfEdge(Traits::DerType& mesh) : m_mesh(mesh) {}
        HalfEdge(){
        }
        
	public:
        Traits::HType* m_next = nullptr;
        Traits::HType* m_previous = nullptr;
        Traits::HType* m_twin = nullptr;
        Traits::VType* m_vertex = nullptr;
        Traits::EType* m_edge = nullptr;
        Traits::FType* m_face = nullptr;
        Traits::DerType& m_mesh;

		int m_halfEdgeIndexInVector = -1;
	};

    template<typename Traits = HalfEdgeTraits>
	class Vertex
	{
	public:
		Vertex(Traits::DerType& mesh) : m_mesh(mesh) {}

	public:
        Traits::HType* m_halfEdge = nullptr;
		glm::vec3 m_position{};
        Traits::DerType& m_mesh;

		int m_vertexIndexInVector = -1;
	};

    template<typename Traits = HalfEdgeTraits>
	class Edge
	{
	public:
		Edge(Traits::DerType& mesh) : m_mesh(mesh) {}

	public:
        Traits::HType* m_halfEdge = nullptr;
        Traits::VType* m_firstVertex = nullptr;
        Traits::VType* m_secondVertex = nullptr;

		int m_edgeIndexInVector = -1;

        bool m_isOuter = false;

        Traits::DerType& m_mesh;
	};

    template<typename Traits = HalfEdgeTraits>
	class Face
	{
	public:
		Face(Traits::DerType& mesh) : m_mesh(mesh) {}

		class FaceVertexIterator {
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Traits::VType;
			using pointer = Traits::VType*;
			using reference = Traits::VType&;

			FaceVertexIterator(Traits::HType* halfEdge)
			{
				m_currentHalfEdge = halfEdge;
				m_startOfHalfEdgeLoop = true;
			}

			FaceVertexIterator& operator++() {
				if (m_startOfHalfEdgeLoop)
				{
					m_startOfHalfEdgeLoop = false;
				}

				m_currentHalfEdge = m_currentHalfEdge->m_next;
				return *this;
			}

            Traits::VType& operator*()
			{
				return *m_currentHalfEdge->m_vertex;
			}

			bool operator==(const FaceVertexIterator& other) const {
				if (m_startOfHalfEdgeLoop) {
					return false;
				}

				return m_currentHalfEdge == other.m_currentHalfEdge;
			}

			bool operator!=(const FaceVertexIterator& other) const {
				return !(*this == other);
			}

			friend long distance(FaceVertexIterator begin, FaceVertexIterator end) {
				int count = 0;
				while (begin != end) {
					++begin;
					++count;
				}
				return count;
			}

		private:
            Traits::HType* m_currentHalfEdge = nullptr;
			bool m_startOfHalfEdgeLoop;
		};

		
		class FaceHalfEdgeIterator {
		public:
			FaceHalfEdgeIterator(Traits::HType* halfEdge)
			{
				m_currentHalfEdge = halfEdge;
				m_startOfHalfEdgeLoop = true;
			}

			FaceHalfEdgeIterator& operator++() {
				if (m_startOfHalfEdgeLoop)
				{
					m_startOfHalfEdgeLoop = false;
				}

				m_currentHalfEdge = m_currentHalfEdge->m_next;
				return *this;
			}

            Traits::HType& operator*() {
				return *m_currentHalfEdge;
			}

			bool operator==(const FaceHalfEdgeIterator& other) const {
				if (m_startOfHalfEdgeLoop)
				{
					return false;
				}
				else
				{
					return m_currentHalfEdge == other.m_currentHalfEdge;
				}
			}

			bool operator!=(const FaceHalfEdgeIterator& other) const {
				return !(*this == other);
			}

		private:
            Traits::HType* m_currentHalfEdge;
			bool m_startOfHalfEdgeLoop;
		};

		FaceHalfEdgeIterator faceHalfEdgeBegin() { return FaceHalfEdgeIterator(m_halfEdge); }
		FaceHalfEdgeIterator faceHalfEdgeEnd() { return FaceHalfEdgeIterator(m_halfEdge); }

		FaceVertexIterator faceVertexBegin() { return FaceVertexIterator(m_halfEdge); }
		FaceVertexIterator faceVertexEnd() { return FaceVertexIterator(m_halfEdge); }

	public:
        Traits::HType* m_halfEdge = nullptr;
        Traits::DerType& m_mesh;

		int m_faceIndexInVector = -1;
	};

    template<typename Traits = HalfEdgeTraits>
	class HalfEdgeMesh
	{
	public:
		HalfEdgeMesh()
		{
		}

		auto halfEdgeIterBegin() { return m_halfEdges.begin(); }
		auto halfEdgeIterEnd() { return m_halfEdges.end(); }

		auto vertexIterBegin() { return m_vertices.begin(); }
		auto vertexIterEnd() { return m_vertices.end(); }

		auto edgeIterBegin() { return m_edges.begin(); }
		auto edgeIterEnd() { return m_edges.end(); }

		auto faceIterBegin() { return m_faces.begin(); }
		auto faceIterEnd() { return m_faces.end(); }

		static void getVerticesFromFace(Traits::FType* face, std::vector<typename Traits::VType>& vertices) {
			vertices.clear();
			
            typename Traits::HType* halfEdge1 = face->m_halfEdge;

            auto halfEdge = face->m_halfEdge;
			do {
                typename Traits::VType* vertex = halfEdge->m_vertex;
				vertices.push_back(*vertex);
				halfEdge = halfEdge->m_next;

			} while (halfEdge != face->m_halfEdge);
		}

		template <typename T, typename Vector>
		T* createObject(Vector& storageVector) {
            T* newObject = nullptr;
            if constexpr (std::is_void_v<typename Traits::DerType>) {
                newObject = new T(*this); 
            } else {
                newObject = new T(static_cast<typename Traits::DerType&>(*this));
            }
			storageVector.emplace_back(newObject);
			return newObject;
		}

		template <typename T, typename Key, typename Map>
		T* findObject(const Key& key, Map& helperMap) {
			auto it = helperMap.find(key);
			return (it != helperMap.end()) ? it->second : nullptr;
		}

		template <typename T, typename Key, typename Map, typename Vector>
		T* findOrCreateObject(const Key& key, Map& helperMap, Vector& storageVector, bool* outWasCreated = nullptr) {
			auto it = helperMap.find(key);
			if (it != helperMap.end()) {
				if (outWasCreated) *outWasCreated = false;
				return it->second;
			}
            T* newObject = nullptr;

            if constexpr (std::is_void_v<typename Traits::DerType>) {
                newObject = new T(*this);
            } else {
                newObject = new T(static_cast<typename Traits::DerType&>(*this));
            }
			helperMap[key] = newObject;
			storageVector.emplace_back(newObject);
			if (outWasCreated) *outWasCreated = true;
			return newObject;
		}

		void build(const std::vector<std::vector<int>>& polygons, const std::vector<glm::vec3>& vertices)
		{
			//HELPER_MAPS_FOR_BUILD
			std::map<std::pair<glm::vec3, glm::vec3>, typename Traits::HType*> helperHalfEdgeMap;
			std::map<glm::vec3, typename Traits::VType*> helperVertexMap;

			for (const std::vector<int>& polygonIndices : polygons)
			{
				//---FACE_CREATION---
                typename Traits::FType* face = createObject<typename Traits::FType>(m_faces);
				face->m_faceIndexInVector = m_faces.size() - 1;

				
				for(int i = 0; i <= polygonIndices.size()-1; ++i)
				{
					glm::vec3 firstVertexPos = vertices.at(polygonIndices.at((i) % polygonIndices.size()));
					glm::vec3 secondVertexPos = vertices.at(polygonIndices.at((i + 1) % polygonIndices.size()));

					//---HALF_EDGE_CREATION
					bool newHalfEdgeCreated = false;
                    typename Traits::HType* halfEdge = 
						findOrCreateObject<typename Traits::HType>(std::make_pair(firstVertexPos, secondVertexPos), helperHalfEdgeMap, m_halfEdges, &newHalfEdgeCreated);
					if (newHalfEdgeCreated) halfEdge->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

					//---VERTEX_CREATION---
					bool newVertexCreated = false;
                    typename Traits::VType* firstVertex = findOrCreateObject<typename Traits::VType>(firstVertexPos, helperVertexMap, m_vertices, &newVertexCreated);
					if(newVertexCreated) firstVertex->m_vertexIndexInVector = m_vertices.size() - 1;
                    typename Traits::VType* secondVertex = findOrCreateObject<typename Traits::VType>(secondVertexPos, helperVertexMap, m_vertices, &newVertexCreated);
					if (newVertexCreated) secondVertex->m_vertexIndexInVector = m_vertices.size() - 1;

					//---VERTEX_INITIALIZATION---
					if (firstVertex && !firstVertex->m_halfEdge) { firstVertex->m_position = firstVertexPos, firstVertex->m_halfEdge = halfEdge; }

					//---FACE_INITIALIZATION---
					if (face && !face->m_halfEdge) face->m_halfEdge = halfEdge;

					//---HALF_EDGE_INITIALIZATION---
						//---SET_STARTING_VERTEX---
					halfEdge->m_vertex = firstVertex;
					//halfEdge->setVertex(firstVertex);

						//---SET_TWIN_HALF_EDGE---
                    typename Traits::HType* twinHalfEdge = findObject<typename Traits::HType>(std::make_pair(secondVertexPos, firstVertexPos), helperHalfEdgeMap);

						//---SET_EDGE---
                    typename Traits::EType* edge = nullptr;
					if(twinHalfEdge)
					{
						halfEdge->m_twin = twinHalfEdge;
						twinHalfEdge->m_twin = halfEdge;
						edge = twinHalfEdge->m_edge;
					} else
					{
						//---EDGE_CREATION---
						edge = createObject<typename Traits::EType>(m_edges);
						edge->m_edgeIndexInVector = m_edges.size() - 1;
 
					}
					halfEdge->m_edge = edge;

						//---SET_NEXT_HALF_EDGE
					glm::vec3 nextFirstVertex =  vertices.at(polygonIndices.at((i + 1) % polygonIndices.size()));
					glm::vec3 nextSecondVertex = vertices.at(polygonIndices.at((i + 2) % polygonIndices.size()));
                    typename Traits::HType* nextHalfEdge = 
						findOrCreateObject<typename Traits::HType>(std::make_pair(nextFirstVertex, nextSecondVertex), helperHalfEdgeMap, m_halfEdges, &newHalfEdgeCreated);
					halfEdge->m_next = nextHalfEdge;

					if (newHalfEdgeCreated) nextHalfEdge->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

						//---SET_PREVIOUS_HALF_EDGE
					glm::vec3 previousFirstVertex = vertices.at(polygonIndices.at((i - 1 + polygonIndices.size()) % polygonIndices.size()));
					glm::vec3 previousSecondVertex = vertices.at(polygonIndices.at((i) % polygonIndices.size()));
                    typename Traits::HType* previousHalfEdge = 
						findOrCreateObject<typename Traits::HType>(std::make_pair(previousFirstVertex, previousSecondVertex), helperHalfEdgeMap, m_halfEdges, &newHalfEdgeCreated);
					halfEdge->m_previous = previousHalfEdge;

					if (newHalfEdgeCreated) previousHalfEdge->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

						//---SET_FACE---
					halfEdge->m_face = face;
					//---HALF_EDGE_INITIALIZED---

					//---EDGE_INITIALIZATION---
					if(edge && !edge->m_halfEdge)
					{
						//---ADDING_NEIGHBOURING_EDGES---
                        //CHANGE
						//firstVertex->m_neighbourEdges.emplace_back(edge);
						//secondVertex->m_neighbourEdges.emplace_back(edge);

						edge->m_halfEdge = halfEdge;
						edge->m_firstVertex = firstVertex;
						edge->m_secondVertex = secondVertex;

                        edge->m_isOuter = true;
					} else if(edge && edge->m_halfEdge) {
                        edge->m_isOuter = false;
                    }
				}
			}
		}

	public:
		std::vector<typename Traits::HType*> m_halfEdges;
		std::vector<typename Traits::VType*> m_vertices;
		std::vector<typename Traits::EType*> m_edges;
        std::vector<typename Traits::FType*> m_faces;
	};
}


