#pragma once

#include <iostream>
#include <vector>
#include "../glm_overrides.h"
#include <unordered_set>
#include <map>

using FaceTriangleIndex = int;
using EdgeLineIndex = int;

namespace HalfEdgeDS
{
	class HalfEdge;
	class Edge;
	class Vertex;
	class Face;
	class HalfEdgeMesh;

	class GraphEdge
	{
	public:
		Vertex* vertex = nullptr;
		int graphEdgeIndexInVertex = -1;
		Face* face = nullptr;
		int graphEdgeIndexInFace = -1;
	};

	class HalfEdge
	{
	public:
		HalfEdge(HalfEdgeMesh& mesh) : m_mesh(mesh) {}

	public:
		HalfEdge* m_next = nullptr;
		HalfEdge* m_previous = nullptr;
		HalfEdge* m_twin = nullptr;
		Vertex* m_vertex = nullptr;
		Edge* m_edge = nullptr;
		Face* m_face = nullptr;

		int m_halfEdgeIndexInVector = -1;

		HalfEdgeMesh& m_mesh;
	};

	class Vertex
	{
	public:
		Vertex(HalfEdgeMesh& mesh) : m_mesh(mesh) {}

	public:
		HalfEdge* m_halfEdge = nullptr;
		glm::vec3 m_position{};
		HalfEdgeMesh& m_mesh;

		int m_vertexIndexInVector = -1;

		//graph
		std::vector<GraphEdge*> m_graphEdges;
	};

	class Edge
	{
	public:
		Edge(HalfEdgeMesh& mesh) : m_mesh(mesh) {}

	public:
		HalfEdge* m_halfEdge = nullptr;
		Vertex* m_firstVertex = nullptr;
		Vertex* m_secondVertex = nullptr;

		int m_edgeIndexInVector = -1;

		EdgeLineIndex m_EdgeLineIndex = -1;
		HalfEdgeMesh& m_mesh;
	};

	class Face
	{
	public:
		Face(HalfEdgeMesh& mesh) : m_mesh(mesh) {}

		class FaceVertexIterator {
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Vertex;
			using pointer = Vertex*;
			using reference = Vertex&;

			FaceVertexIterator(HalfEdge* halfEdge)
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

			Vertex& operator*()
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
			HalfEdge* m_currentHalfEdge = nullptr;
			bool m_startOfHalfEdgeLoop;
		};

		
		class FaceHalfEdgeIterator {
		public:
			FaceHalfEdgeIterator(HalfEdge* halfEdge)
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

			HalfEdge& operator*() {
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
			HalfEdge* m_currentHalfEdge;
			bool m_startOfHalfEdgeLoop;
		};

		FaceHalfEdgeIterator faceHalfEdgeBegin() { return FaceHalfEdgeIterator(m_halfEdge); }
		FaceHalfEdgeIterator faceHalfEdgeEnd() { return FaceHalfEdgeIterator(m_halfEdge); }

		FaceVertexIterator faceVertexBegin() { return FaceVertexIterator(m_halfEdge); }
		FaceVertexIterator faceVertexEnd() { return FaceVertexIterator(m_halfEdge); }

	public:
		HalfEdge* m_halfEdge = nullptr;
		HalfEdgeMesh& m_mesh;

		//selection
		bool m_selected = false;
		int m_selectionIndex = -1;

		int m_faceIndexInVector = -1;

		//graph
		std::vector<GraphEdge*> m_graphEdges;
		Material* material = nullptr;
		std::vector<FaceTriangleIndex> faceTriangleIndices;
	};

	struct FaceTriangle
	{
		int indexInVAO = -1;
		Face* face = nullptr;
		int indexInFace = -1;
	};



	class HalfEdgeMesh
	{
	private:

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

		void getVerticesFromFace(Face* face, std::vector<Vertex>& vertices) {
			vertices.clear();
			
			HalfEdge* halfEdge = face->m_halfEdge;
			do {
				Vertex* vertex = halfEdge->m_vertex;
				vertices.push_back(*vertex);
				halfEdge = halfEdge->m_next;

			} while (halfEdge != face->m_halfEdge);
		}

		template <typename T, typename Vector>
		T* createObject(Vector& storageVector) {
			auto newObject = new T(*this);
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

			auto newObject = new T(*this);
			helperMap[key] = newObject;
			storageVector.emplace_back(newObject);
			if (outWasCreated) *outWasCreated = true;
			return newObject;
		}

		void buildGraph()
		{
			//---GO_THROUGH_ALL_FACES---
			for (Face* face : m_faces)
			{
				HalfEdge* halfEdge = face->m_halfEdge;
				do {
					//---ACCESS_EACH_VERTEX_OF_FACE---
					Vertex* vertex = halfEdge->m_vertex;
					halfEdge = halfEdge->m_next;

					//---CREATE_GRAPH_EDGE---
					GraphEdge* graphEdge = new GraphEdge();
					graphEdge->vertex = vertex;
					graphEdge->graphEdgeIndexInVertex = vertex->m_graphEdges.size();

					graphEdge->face = face;
					graphEdge->graphEdgeIndexInFace = face->m_graphEdges.size();

					vertex->m_graphEdges.emplace_back(graphEdge);
					face->m_graphEdges.emplace_back(graphEdge);

				} while (halfEdge != face->m_halfEdge);
			}

			std::cout << "Graph built" << std::endl;
		}

		void build(const std::vector<std::vector<int>>& polygons, const std::vector<glm::vec3>& vertices)
		{
			//HELPER_MAPS_FOR_BUILD
			std::map<std::pair<glm::vec3, glm::vec3>, HalfEdge*> helperHalfEdgeMap;
			std::map<glm::vec3, Vertex*> helperVertexMap;

			for (const std::vector<int>& polygonIndices : polygons)
			{
				//---FACE_CREATION---
				Face* face = createObject<Face>(m_faces);
				face->m_faceIndexInVector = m_faces.size() - 1;

				
				for(int i = 0; i <= polygonIndices.size()-1; ++i)
				{
					glm::vec3 firstVertexPos = vertices.at(polygonIndices.at((i) % polygonIndices.size()));
					glm::vec3 secondVertexPos = vertices.at(polygonIndices.at((i + 1) % polygonIndices.size()));

					//---HALF_EDGE_CREATION
					bool newHalfEdgeCreated = false;
					HalfEdge* halfEdge = 
						findOrCreateObject<HalfEdge>(std::make_pair(firstVertexPos, secondVertexPos), helperHalfEdgeMap, m_halfEdges, &newHalfEdgeCreated);
					if (newHalfEdgeCreated) halfEdge->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

					//---VERTEX_CREATION---
					bool newVertexCreated = false;
					Vertex* firstVertex = findOrCreateObject<Vertex>(firstVertexPos, helperVertexMap, m_vertices, &newVertexCreated);
					if(newVertexCreated) firstVertex->m_vertexIndexInVector = m_vertices.size() - 1;
					Vertex* secondVertex = findOrCreateObject<Vertex>(secondVertexPos, helperVertexMap, m_vertices, &newVertexCreated);
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
					HalfEdge* twinHalfEdge = findObject<HalfEdge>(std::make_pair(secondVertexPos, firstVertexPos), helperHalfEdgeMap);

						//---SET_EDGE---
					Edge* edge = nullptr;
					if(twinHalfEdge)
					{
						halfEdge->m_twin = twinHalfEdge;
						twinHalfEdge->m_twin = halfEdge;
						edge = twinHalfEdge->m_edge;
					} else
					{
						//---EDGE_CREATION---
						edge = createObject<Edge>(m_edges);
						edge->m_edgeIndexInVector = m_edges.size() - 1;
					}
					halfEdge->m_edge = edge;

						//---SET_NEXT_HALF_EDGE
					glm::vec3 nextFirstVertex =  vertices.at(polygonIndices.at((i + 1) % polygonIndices.size()));
					glm::vec3 nextSecondVertex = vertices.at(polygonIndices.at((i + 2) % polygonIndices.size()));
					HalfEdge* nextHalfEdge = 
						findOrCreateObject<HalfEdge>(std::make_pair(nextFirstVertex, nextSecondVertex), helperHalfEdgeMap, m_halfEdges, &newHalfEdgeCreated);
					halfEdge->m_next = nextHalfEdge;

					if (newHalfEdgeCreated) nextHalfEdge->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

						//---SET_PREVIOUS_HALF_EDGE
					glm::vec3 previousFirstVertex = vertices.at(polygonIndices.at((i - 1 + polygonIndices.size()) % polygonIndices.size()));
					glm::vec3 previousSecondVertex = vertices.at(polygonIndices.at((i) % polygonIndices.size()));
					HalfEdge* previousHalfEdge = 
						findOrCreateObject<HalfEdge>(std::make_pair(previousFirstVertex, previousSecondVertex), helperHalfEdgeMap, m_halfEdges, &newHalfEdgeCreated);
					halfEdge->m_previous = previousHalfEdge;

					if (newHalfEdgeCreated) previousHalfEdge->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

						//---SET_FACE---
					halfEdge->m_face = face;
					//---HALF_EDGE_INITIALIZED---

					//---EDGE_INITIALIZATION---
					if(edge && !edge->m_halfEdge)
					{
						edge->m_halfEdge = halfEdge;
						edge->m_firstVertex = firstVertex;
						edge->m_secondVertex = secondVertex;
					}
				}
			}

			//---BUILD_GRAPH---
			buildGraph();
		}

	public:
		std::vector<HalfEdge*> m_halfEdges;
		std::vector<Vertex*> m_vertices;
		std::vector<Edge*> m_edges;
		std::vector<Face*> m_faces;

		std::map<Material*, std::vector<FaceTriangle>> m_faceTriangles;
	};
}