#pragma once

#include <iostream>
#include <vector>
#include "../glm_comparisons.h"


#include <map>

#include "../Renderer/VertexDataStructs.h"


namespace HalfEdgeDS
{
	class HalfEdge;
	class Edge;
	class Vertex;
	class Face;
	class HalfEdgeMesh;

	inline size_t MAX_INDEX_VAL = std::numeric_limits<size_t>::max();

	using HalfEdgeIndex = size_t;
	using EdgeIndex = size_t;
	using VertexIndex = size_t;
	using FaceIndex = size_t;

	using HalfEdgeIter = std::vector<HalfEdge>::iterator;
	using EdgeIter = std::vector<Edge>::iterator;
	using VertexIter = std::vector<Vertex>::iterator;
	using FaceIter = std::vector<Face>::iterator;


	class HalfEdge
	{
	public:
		HalfEdge(HalfEdgeMesh& mesh) : m_mesh(mesh) {}
		
		void setNext(HalfEdgeIndex next) { m_next = next; }
		void setPrevious(HalfEdgeIndex previous) { m_previous = previous; }
		void setTwin(HalfEdgeIndex twin) { m_twin = twin; }
		void setVertex(VertexIndex vertex) { m_vertex = vertex; }
		void setEdge(EdgeIndex edge) { m_edge = edge; }
		void setFace(FaceIndex face) { m_face = face; }

		HalfEdgeIter getNext();
		HalfEdgeIter getPrevious();
		HalfEdgeIter getTwin();
		VertexIter getVertex();
		EdgeIter getEdge();
		FaceIter getFace();

		HalfEdgeIndex getNextIndex() const { return m_next; }
		HalfEdgeIndex getPreviousIndex() const { return m_previous; }
		HalfEdgeIndex getTwinIndex() const { return m_twin; }
		VertexIndex getVertexIndex() const { return m_vertex; }
		EdgeIndex getEdgeIndex() const { return m_edge; }
		FaceIndex getFaceIndex() const { return m_face; }

		HalfEdgeMesh& getMesh() { return m_mesh; }

		bool operator==(const HalfEdge& other) const {
			return m_next == other.m_next &&
				m_previous == other.m_previous &&
				m_twin == other.m_twin &&
				m_vertex == other.m_vertex &&
				m_edge == other.m_edge &&
				m_face == other.m_face;
		}

		bool operator!=(const HalfEdge& other) const {
			return !(*this == other);
		}

	private:
		HalfEdgeIndex m_next = MAX_INDEX_VAL;
		HalfEdgeIndex m_previous = MAX_INDEX_VAL;
		HalfEdgeIndex m_twin = MAX_INDEX_VAL;
		VertexIndex m_vertex = MAX_INDEX_VAL;
		EdgeIndex m_edge = MAX_INDEX_VAL;
		FaceIndex m_face = MAX_INDEX_VAL;

		HalfEdgeMesh& m_mesh;
	};

	class Vertex
	{
	public:
		Vertex(HalfEdgeMesh& mesh) : m_mesh(mesh) {}
	
		void setHalfEdge(HalfEdgeIndex halfEdge) { m_halfEdge = halfEdge; }
		void setPosition(const glm::vec3& position) { m_position = position; }

		HalfEdgeIter getHalfEdge();

		HalfEdgeIndex getHalfEdgeIndex() const { return m_halfEdge; }

		glm::vec3& getPosition() { return m_position; }

		HalfEdgeMesh& getMesh() { return m_mesh; }

		bool operator==(const Vertex& other) const {
			return m_halfEdge == other.m_halfEdge;
		}

		bool operator!=(const Vertex& other) const {
			return !(*this == other);
		}

	private:
		HalfEdgeIndex m_halfEdge = MAX_INDEX_VAL;
		glm::vec3 m_position{};

		HalfEdgeMesh& m_mesh;
	};

	class Edge
	{
	public:
		Edge(HalfEdgeMesh& mesh) : m_mesh(mesh) {}

		void setHalfEdge(HalfEdgeIndex halfEdge) { m_halfEdge = halfEdge; }
		void setFirstVertex(VertexIndex firstVertex) { m_firstVertex = firstVertex; }
		void setSecondVertex(VertexIndex secondVertex) { m_secondVertex = secondVertex; }
		
		HalfEdgeIter getHalfEdge();
		VertexIter getFirstVertex();
		VertexIter getSecondVertex();

		HalfEdgeIndex getHalfEdgeIndex() const { return m_halfEdge; }
		HalfEdgeIndex getFirstVertexIndex() const { return m_firstVertex; }
		HalfEdgeIndex getSecondVertexIndex() const { return m_secondVertex; }

		HalfEdgeMesh& getMesh() { return m_mesh; }

		bool operator==(const Edge& other) const {
			return m_halfEdge == other.m_halfEdge;
		}

		bool operator!=(const Edge& other) const {
			return !(*this == other);
		}

	private:
		HalfEdgeIndex m_halfEdge = MAX_INDEX_VAL;
		VertexIndex m_firstVertex = MAX_INDEX_VAL;
		VertexIndex m_secondVertex = MAX_INDEX_VAL;

		HalfEdgeMesh& m_mesh;
	};

	class Face
	{
	public:
		Face(HalfEdgeMesh& mesh) : m_mesh(mesh) {}

		class FaceVertexIterator {
		public:
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
				m_currentHalfEdge = &m_currentHalfEdge->getNext().operator*();

				return *this;
			}

			Vertex& operator*() {
				return m_currentHalfEdge->getVertex().operator*();
			}

			bool operator==(const FaceVertexIterator& other) const {
				if (m_startOfHalfEdgeLoop)
				{
					return false;
				}
				else
				{
					return m_currentHalfEdge == other.m_currentHalfEdge;
				}
			}

			bool operator!=(const FaceVertexIterator& other) const {
				return !(*this == other);
			}

		private:
			HalfEdge* m_currentHalfEdge;
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
				m_currentHalfEdge = &m_currentHalfEdge->getNext().operator*();

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

		FaceHalfEdgeIterator faceHalfEdgeBegin() { return FaceHalfEdgeIterator(&getHalfEdge().operator*()); }
		FaceHalfEdgeIterator faceHalfEdgeEnd() { return FaceHalfEdgeIterator(&getHalfEdge().operator*()); }
		

		FaceVertexIterator faceVertexBegin() { return FaceVertexIterator(&getHalfEdge().operator*()); }
		FaceVertexIterator faceVertexEnd() { return FaceVertexIterator(&getHalfEdge().operator*()); }


		void setHalfEdge(HalfEdgeIndex halfEdge) { m_halfEdge = halfEdge; }
		
		HalfEdgeIter getHalfEdge();

		HalfEdgeIndex getHalfEdgeIndex() const { return m_halfEdge; }

		HalfEdgeMesh& getMesh() { return m_mesh; }

	private:
		HalfEdgeIndex m_halfEdge = MAX_INDEX_VAL;
		HalfEdgeMesh& m_mesh;
	};


	class HalfEdgeMesh
	{
	private:

		struct PolygonBuildVariablesState {
			FaceIndex faceIndexOfCurrentPolygon;
			size_t currentVertexIndexInPolygon;
			size_t previousVertexIndexInPolygon;
			HalfEdgeIndex firstHalfEdgeIndexInCurrentPolygon;
			VertexIndex firstVertexIndexInCurrentPolygon;
			bool firstVertexInPolygon;
			bool firstHalfEdgeInPolygon;
			bool isFaceConnectedToHalfEdge;

			HalfEdgeIndex currentHalfEdgeIndexInPolygon;
			HalfEdgeIndex currentTwinHalfEdgeIndexInPolygon;
			HalfEdgeIndex previousHalfEdgeIndexInPolygon;
			EdgeIndex currentEdgeIndexInPolygon;
		};

		VertexIndex addNewVertex()
		{
			m_vertices.emplace_back(std::move(Vertex(*this)));
			VertexIndex vertexIndex = m_vertices.size() - 1;
			return vertexIndex;
		}

		HalfEdgeIndex addNewHalfEdge()
		{
			m_halfEdges.emplace_back(std::move(HalfEdge(*this)));
			HalfEdgeIndex halfEdgeIndex = m_halfEdges.size() - 1;
			return halfEdgeIndex;
		}

		FaceIndex addNewFace()
		{
			m_faces.emplace_back(std::move(Face(*this)));
			FaceIndex faceIndex = m_faces.size() - 1;
			return faceIndex;
		}

		EdgeIndex addNewEdge()
		{
			m_edges.emplace_back(std::move(Edge(*this)));
			EdgeIndex edgeIndex = m_edges.size() - 1;
			return edgeIndex;
		}

		void setPositionOfVertex(VertexIndex vertexIndex, const glm::vec3& vertexPosition)
		{
			m_vertices.at(vertexIndex).setPosition(vertexPosition);
		}

		void setVariablesForAddedPolygon(PolygonBuildVariablesState& buildState)
		{
			buildState.faceIndexOfCurrentPolygon = addNewFace();
			buildState.currentVertexIndexInPolygon = MAX_INDEX_VAL;
			buildState.previousVertexIndexInPolygon = MAX_INDEX_VAL;
			buildState.firstHalfEdgeIndexInCurrentPolygon = MAX_INDEX_VAL;
			buildState.firstVertexIndexInCurrentPolygon = MAX_INDEX_VAL;
			buildState.firstVertexInPolygon = true;
			buildState.firstHalfEdgeInPolygon = true;
			buildState.isFaceConnectedToHalfEdge = false;

			buildState.currentHalfEdgeIndexInPolygon = MAX_INDEX_VAL;
			buildState.previousHalfEdgeIndexInPolygon = MAX_INDEX_VAL;
			buildState.currentEdgeIndexInPolygon = MAX_INDEX_VAL;
		}

		HalfEdgeIndex findTwinOfHalfEdge(std::map<std::pair<glm::vec3, glm::vec3>, int>& halfEdgeMap, const glm::vec3& firstVertex, const glm::vec3& secondVertex)
		{
			auto halfEdgeFirstTwinIt = halfEdgeMap.find({ std::make_pair(firstVertex, secondVertex)});
			auto halfEdgeSecondTwinIt = halfEdgeMap.find({ std::make_pair(secondVertex, firstVertex) });

			HalfEdgeIndex halfEdgeTwinIndex = MAX_INDEX_VAL;
			if (halfEdgeFirstTwinIt != halfEdgeMap.end())
			{
				halfEdgeTwinIndex = halfEdgeFirstTwinIt->second;
			}
			else if (halfEdgeSecondTwinIt != halfEdgeMap.end())
			{
				halfEdgeTwinIndex = halfEdgeSecondTwinIt->second;
			}
			return halfEdgeTwinIndex;
		}

		//DATA FOR HALFEDGE
		void setVertexDataForHalfEdge(HalfEdgeIndex halfEdgeIndex, VertexIndex vertexIndex)
		{
			m_halfEdges.at(halfEdgeIndex).setVertex(vertexIndex);
		}

		void setFaceDataForHalfEdge(HalfEdgeIndex halfEdgeIndex, FaceIndex faceIndex)
		{
			m_halfEdges.at(halfEdgeIndex).setFace(faceIndex);
		}

		void setTwinDataForHalfEdge(HalfEdgeIndex halfEdgeIndex, HalfEdgeIndex twinHalfEdgeIndex)
		{
			m_halfEdges.at(halfEdgeIndex).setTwin(twinHalfEdgeIndex);
			m_halfEdges.at(twinHalfEdgeIndex).setTwin(halfEdgeIndex);
		}

		void setPreviousDataForHalfEdge(HalfEdgeIndex halfEdgeIndex, HalfEdgeIndex previousHalfEdgeIndex)
		{
			m_halfEdges.at(halfEdgeIndex).setPrevious(previousHalfEdgeIndex);
		}

		void setNextDataForHalfEdge(HalfEdgeIndex halfEdgeIndex, HalfEdgeIndex nextHalfEdgeIndex)
		{
			m_halfEdges.at(halfEdgeIndex).setNext(nextHalfEdgeIndex);
		}

		void setEdgeDataForHalfEdge(HalfEdgeIndex halfEdgeIndex, EdgeIndex edgeIndex)
		{
			m_halfEdges.at(halfEdgeIndex).setEdge(edgeIndex);
		}

		EdgeIndex getEdgeDataOfHalfEdge(HalfEdgeIndex halfEdgeIndex)
		{
			return m_halfEdges.at(halfEdgeIndex).getEdgeIndex();
		}
		//

		//DATA FOR EDGE
		void setVerticesDataForEdge(EdgeIndex edgeIndex, VertexIndex firstVertex, VertexIndex secondVertex)
		{
			m_edges.at(edgeIndex).setFirstVertex(firstVertex);
			m_edges.at(edgeIndex).setSecondVertex(secondVertex);
		}

		void setHalfEdgeDataForEdge(EdgeIndex edgeIndex, HalfEdgeIndex halfEdgeIndex)
		{
			m_edges.at(edgeIndex).setHalfEdge(halfEdgeIndex);
		}
		//

		//DATA FOR FACE
		void setHalfEdgeDataForFace(FaceIndex faceIndex, HalfEdgeIndex halfEdgeIndex)
		{
			m_faces.at(faceIndex).setHalfEdge(halfEdgeIndex);
		}
		//

		//DATA FOR VERTEX
		HalfEdgeIndex getHalfEdgeDataOfVertex(VertexIndex vertexIndex)
		{
			return m_vertices.at(vertexIndex).getHalfEdgeIndex();
		}

		void setHalfEdgeDataForVertex(VertexIndex vertexIndex, HalfEdgeIndex halfEdgeIndex)
		{
			m_vertices.at(vertexIndex).setHalfEdge(halfEdgeIndex);
		}
		//

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


		std::vector<Vertex>& getVerticesFromFace(FaceIter face) {
			std::vector<Vertex> vertices;

			HalfEdgeIter halfEdge = face->getHalfEdge();
			do {
				VertexIter vertex = halfEdge->getVertex();
				vertices.push_back(*vertex);
				halfEdge = halfEdge->getNext();
			} while (halfEdge != face->getHalfEdge());

			return vertices;
		}

		void build(const std::vector<std::vector<int>>& polygons, const std::vector<glm::vec3>& vertices)
		{

			std::map<glm::vec3, int> vertexMap;
			std::map<std::pair<glm::vec3, glm::vec3>, int> halfEdgeMap;
			PolygonBuildVariablesState polygonState{};

			for (const std::vector<int>& polygon : polygons)
			{
				setVariablesForAddedPolygon(polygonState);

				for (auto& vertIndexInLoop : polygon)
				{
					auto vertexIt = vertexMap.find(vertices.at(vertIndexInLoop));
					if (!polygonState.firstVertexInPolygon)
					{
						polygonState.previousVertexIndexInPolygon = polygonState.currentVertexIndexInPolygon;
					}
					if (vertexIt == vertexMap.end())
					{
						VertexIndex vertexIndex = addNewVertex();
						setPositionOfVertex(vertexIndex, vertices.at(vertIndexInLoop));
						polygonState.currentVertexIndexInPolygon = vertexIndex;

						vertexMap.insert({ vertices.at(vertIndexInLoop), polygonState.currentVertexIndexInPolygon });
					}
					else
					{
						polygonState.currentVertexIndexInPolygon = vertexIt->second;
					}
					if (polygonState.firstVertexInPolygon)
					{
						polygonState.firstVertexInPolygon = false;
						polygonState.firstVertexIndexInCurrentPolygon = polygonState.currentVertexIndexInPolygon;
					}
					else
					{
						
						polygonState.currentHalfEdgeIndexInPolygon = addNewHalfEdge();

						polygonState.currentTwinHalfEdgeIndexInPolygon = findTwinOfHalfEdge(halfEdgeMap, m_vertices.at(polygonState.currentVertexIndexInPolygon).getPosition(), m_vertices.at(polygonState.previousVertexIndexInPolygon).getPosition());

						if(polygonState.currentTwinHalfEdgeIndexInPolygon == MAX_INDEX_VAL)
						{
							halfEdgeMap.insert({ std::make_pair(m_vertices.at(polygonState.previousVertexIndexInPolygon).getPosition(), m_vertices.at(polygonState.currentVertexIndexInPolygon).getPosition()), polygonState.currentHalfEdgeIndexInPolygon });

							polygonState.currentEdgeIndexInPolygon = addNewEdge();
							setHalfEdgeDataForEdge(polygonState.currentEdgeIndexInPolygon, polygonState.currentHalfEdgeIndexInPolygon);
							setVerticesDataForEdge(polygonState.currentEdgeIndexInPolygon, polygonState.currentVertexIndexInPolygon, polygonState.previousVertexIndexInPolygon);
							setEdgeDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, polygonState.currentEdgeIndexInPolygon);

							setHalfEdgeDataForVertex(polygonState.previousVertexIndexInPolygon, polygonState.currentHalfEdgeIndexInPolygon);

						} else
						{
							setTwinDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, polygonState.currentTwinHalfEdgeIndexInPolygon);

							EdgeIndex twinEdgeIndex = getEdgeDataOfHalfEdge(m_halfEdges.at(polygonState.currentHalfEdgeIndexInPolygon).getTwinIndex());

							setEdgeDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, twinEdgeIndex);
						}

						setVertexDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, polygonState.previousVertexIndexInPolygon);

						setFaceDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, polygonState.faceIndexOfCurrentPolygon);


						if (polygonState.firstHalfEdgeInPolygon)
						{
							polygonState.firstHalfEdgeInPolygon = false;
							polygonState.firstHalfEdgeIndexInCurrentPolygon = polygonState.currentHalfEdgeIndexInPolygon;
						}
						else
						{
							polygonState.previousHalfEdgeIndexInPolygon = m_halfEdges.size() - 2;
							setNextDataForHalfEdge(polygonState.previousHalfEdgeIndexInPolygon, polygonState.currentHalfEdgeIndexInPolygon);
							setPreviousDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, polygonState.previousHalfEdgeIndexInPolygon);
						}

						if (!polygonState.isFaceConnectedToHalfEdge)
						{
							setHalfEdgeDataForFace(polygonState.faceIndexOfCurrentPolygon, polygonState.currentHalfEdgeIndexInPolygon);
							polygonState.isFaceConnectedToHalfEdge = true;
						}


						if (vertIndexInLoop == polygon.at(polygon.size() - 1))
						{
							polygonState.previousHalfEdgeIndexInPolygon = polygonState.currentHalfEdgeIndexInPolygon;
							polygonState.currentHalfEdgeIndexInPolygon = addNewHalfEdge();

							setVertexDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, polygonState.currentVertexIndexInPolygon);

							if(getHalfEdgeDataOfVertex(polygonState.currentVertexIndexInPolygon) == MAX_INDEX_VAL)
							{
								setHalfEdgeDataForVertex(polygonState.currentVertexIndexInPolygon, polygonState.currentHalfEdgeIndexInPolygon);
							}

							setNextDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, polygonState.firstHalfEdgeIndexInCurrentPolygon);

							polygonState.previousHalfEdgeIndexInPolygon = m_halfEdges.size() - 2;
							setPreviousDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, polygonState.previousHalfEdgeIndexInPolygon);

							setPreviousDataForHalfEdge(polygonState.firstHalfEdgeIndexInCurrentPolygon, polygonState.currentHalfEdgeIndexInPolygon);

							setNextDataForHalfEdge(polygonState.previousHalfEdgeIndexInPolygon, polygonState.currentHalfEdgeIndexInPolygon);
							setFaceDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, polygonState.faceIndexOfCurrentPolygon);
						
							polygonState.currentTwinHalfEdgeIndexInPolygon = findTwinOfHalfEdge(halfEdgeMap, m_vertices.at(polygonState.currentVertexIndexInPolygon).getPosition(), m_vertices.at(polygonState.firstVertexIndexInCurrentPolygon).getPosition());

							if (polygonState.currentTwinHalfEdgeIndexInPolygon == MAX_INDEX_VAL)
							{
								halfEdgeMap.insert({ std::make_pair(m_vertices.at(polygonState.currentVertexIndexInPolygon).getPosition(), m_vertices.at(polygonState.firstVertexIndexInCurrentPolygon).getPosition()), polygonState.currentHalfEdgeIndexInPolygon });

								polygonState.currentEdgeIndexInPolygon = addNewEdge();
								setHalfEdgeDataForEdge(polygonState.currentEdgeIndexInPolygon, polygonState.currentHalfEdgeIndexInPolygon);
								setVerticesDataForEdge(polygonState.currentEdgeIndexInPolygon, polygonState.firstVertexIndexInCurrentPolygon, polygonState.currentVertexIndexInPolygon);
								setEdgeDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, polygonState.currentEdgeIndexInPolygon);

							} else
							{
								setTwinDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, polygonState.currentTwinHalfEdgeIndexInPolygon);

								EdgeIndex twinEdgeIndex = getEdgeDataOfHalfEdge(m_halfEdges.at(polygonState.currentHalfEdgeIndexInPolygon).getTwinIndex());
								setEdgeDataForHalfEdge(polygonState.currentHalfEdgeIndexInPolygon, twinEdgeIndex);
							}
						}
					}

				}

			}
		}


	public:
		std::vector<HalfEdge> m_halfEdges;
		std::vector<Vertex> m_vertices;
		std::vector<Edge> m_edges;
		std::vector<Face> m_faces;
	};

	//HALFEDGE
	inline HalfEdgeIter HalfEdge::getNext()
	{
		if (m_next == MAX_INDEX_VAL) {
			return m_mesh.halfEdgeIterEnd();
		}
		return m_mesh.halfEdgeIterBegin() + m_next;
	}

	inline HalfEdgeIter HalfEdge::getPrevious()
	{
		if (m_previous == MAX_INDEX_VAL) {
			return m_mesh.halfEdgeIterEnd();
		}
		return m_mesh.halfEdgeIterBegin() + m_previous;
	}

	inline HalfEdgeIter HalfEdge::getTwin()
	{
		if (m_twin == MAX_INDEX_VAL) {
			return m_mesh.halfEdgeIterEnd();
		}
		return m_mesh.halfEdgeIterBegin() + m_twin;
	}
	inline VertexIter HalfEdge::getVertex()
	{
		if (m_vertex == MAX_INDEX_VAL) {
			return m_mesh.vertexIterEnd();
		}
		return m_mesh.vertexIterBegin() + m_vertex;
	}
	inline EdgeIter HalfEdge::getEdge()
	{
		if (m_edge == MAX_INDEX_VAL) {
			return m_mesh.edgeIterEnd();
		}
		return m_mesh.edgeIterBegin() + m_edge;
	}
	inline FaceIter HalfEdge::getFace()
	{
		if (m_face == MAX_INDEX_VAL) {
			return m_mesh.faceIterEnd();
		}
		return m_mesh.faceIterBegin() + m_face;
	}
	//VERTEX
	inline HalfEdgeIter Vertex::getHalfEdge()
	{
		if (m_halfEdge == MAX_INDEX_VAL) {
			return m_mesh.halfEdgeIterEnd();
		}
		return m_mesh.halfEdgeIterBegin() + m_halfEdge;
	}
	//EDGE
	inline HalfEdgeIter Edge::getHalfEdge()
	{
		if (m_halfEdge == MAX_INDEX_VAL) {
			return m_mesh.halfEdgeIterEnd();
		}
		return m_mesh.halfEdgeIterBegin() + m_halfEdge;
	}
	inline VertexIter Edge::getFirstVertex()
	{
		if (m_firstVertex == MAX_INDEX_VAL) {
			return m_mesh.vertexIterEnd();
		}
		return m_mesh.vertexIterBegin() + m_firstVertex;
	}
	inline VertexIter Edge::getSecondVertex()
	{
		if (m_secondVertex == MAX_INDEX_VAL) {
			return m_mesh.vertexIterEnd();
		}
		return m_mesh.vertexIterBegin() + m_secondVertex;
	}
	//FACE
	inline HalfEdgeIter Face::getHalfEdge()
	{
		if (m_halfEdge == MAX_INDEX_VAL) {
			return m_mesh.halfEdgeIterEnd();
		}
		return m_mesh.halfEdgeIterBegin() + m_halfEdge;
	}

}