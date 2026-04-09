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
	template <typename Traits>
	class HalfEdge;
	template <typename Traits>
	class Edge;
	template <typename Traits>
	class Vertex;
	template <typename Traits>
	class Face;
	template <typename Traits>
	class HalfEdgeMesh;

	struct HalfEdgeTraits
	{
		using FType = Face<HalfEdgeTraits>;
		using EType = Edge<HalfEdgeTraits>;
		using HType = HalfEdge<HalfEdgeTraits>;
		using VType = Vertex<HalfEdgeTraits>;
		using DerType = HalfEdgeMesh<HalfEdgeTraits>;
	};

	template <typename Traits = HalfEdgeTraits>
	class HalfEdge
	{
	public:
		HalfEdge(Traits::DerType &mesh) : m_mesh(mesh) {}
		HalfEdge()
		{
		}

	public:
		Traits::HType *m_next = nullptr;
		Traits::HType *m_previous = nullptr;
		Traits::HType *m_twin = nullptr;
		Traits::VType *m_vertex = nullptr;
		Traits::EType *m_edge = nullptr;
		Traits::FType *m_face = nullptr;
		Traits::DerType &m_mesh;

		int m_halfEdgeIndexInVector = -1;
	};

	template <typename Traits = HalfEdgeTraits>
	class Vertex
	{
	public:
		Vertex(Traits::DerType &mesh) : m_mesh(mesh) {}

	public:
		Traits::HType *m_halfEdge = nullptr;
		glm::vec3 m_position{};
		Traits::DerType &m_mesh;

		int m_vertexIndexInVector = -1;
	};

	template <typename Traits = HalfEdgeTraits>
	class Edge
	{
	public:
		Edge(Traits::DerType &mesh) : m_mesh(mesh) {}

	public:
		Traits::HType *m_halfEdge = nullptr;
		Traits::VType *m_firstVertex = nullptr;
		Traits::VType *m_secondVertex = nullptr;

		int m_edgeIndexInVector = -1;

		bool m_isOuter = false;

		Traits::DerType &m_mesh;
	};

	template <typename Traits = HalfEdgeTraits>
	class Face
	{
	public:
		Face(Traits::DerType &mesh) : m_mesh(mesh) {}

		class FaceVertexIterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Traits::VType;
			using pointer = Traits::VType *;
			using reference = Traits::VType &;

			std::unordered_set<void *> visited; // to detect loops in the face structure

			FaceVertexIterator(Traits::HType *halfEdge)
			{
				m_currentHalfEdge = halfEdge;
				m_startOfHalfEdgeLoop = true;
			}

			FaceVertexIterator &operator++()
			{
				if (m_startOfHalfEdgeLoop)
				{
					m_startOfHalfEdgeLoop = false;
				}

				m_currentHalfEdge = m_currentHalfEdge->m_next;

				if (visited.count((void *)m_currentHalfEdge) > 0)
				{
					// throw std::runtime_error("Loop detected in face structure. Invalid half-edge configuration.");
					printf("Loop detected in face structure. Invalid half-edge configuration.\n");
				}
				visited.insert((void *)m_currentHalfEdge);

				return *this;
			}

			Traits::VType &operator*()
			{
				return *m_currentHalfEdge->m_vertex;
			}

			bool operator==(const FaceVertexIterator &other) const
			{
				if (m_startOfHalfEdgeLoop)
				{
					return false;
				}

				return m_currentHalfEdge == other.m_currentHalfEdge;
			}

			bool operator!=(const FaceVertexIterator &other) const
			{
				return !(*this == other);
			}

			friend long distance(FaceVertexIterator begin, FaceVertexIterator end)
			{
				int count = 0;
				while (begin != end)
				{
					++begin;
					++count;
				}
				return count;
			}

		private:
			Traits::HType *m_currentHalfEdge = nullptr;
			bool m_startOfHalfEdgeLoop;
		};

		class FaceHalfEdgeIterator
		{
		public:
			FaceHalfEdgeIterator(Traits::HType *halfEdge)
			{
				m_currentHalfEdge = halfEdge;
				m_startOfHalfEdgeLoop = true;
			}

			FaceHalfEdgeIterator &operator++()
			{
				if (m_startOfHalfEdgeLoop)
				{
					m_startOfHalfEdgeLoop = false;
				}

				m_currentHalfEdge = m_currentHalfEdge->m_next;
				return *this;
			}

			Traits::HType &operator*()
			{
				return *m_currentHalfEdge;
			}

			bool operator==(const FaceHalfEdgeIterator &other) const
			{
				if (m_startOfHalfEdgeLoop)
				{
					return false;
				}
				else
				{
					return m_currentHalfEdge == other.m_currentHalfEdge;
				}
			}

			bool operator!=(const FaceHalfEdgeIterator &other) const
			{
				return !(*this == other);
			}

		private:
			Traits::HType *m_currentHalfEdge;
			bool m_startOfHalfEdgeLoop;
		};

		FaceHalfEdgeIterator faceHalfEdgeBegin() { return FaceHalfEdgeIterator(m_halfEdge); }
		FaceHalfEdgeIterator faceHalfEdgeEnd() { return FaceHalfEdgeIterator(m_halfEdge); }

		FaceVertexIterator faceVertexBegin() { return FaceVertexIterator(m_halfEdge); }
		FaceVertexIterator faceVertexEnd() { return FaceVertexIterator(m_halfEdge); }

	public:
		Traits::HType *m_halfEdge = nullptr;
		Traits::DerType &m_mesh;

		int m_faceIndexInVector = -1;
	};

	template <typename Traits = HalfEdgeTraits>
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

		static void getVerticesFromFace(Traits::FType *face, std::vector<typename Traits::VType> &vertices)
		{
			vertices.clear();

			typename Traits::HType *halfEdge1 = face->m_halfEdge;

			auto halfEdge = face->m_halfEdge;
			do
			{
				typename Traits::VType *vertex = halfEdge->m_vertex;
				vertices.push_back(*vertex);
				halfEdge = halfEdge->m_next;

			} while (halfEdge != face->m_halfEdge);
		}

		template <typename T, typename Vector>
		T *createObject(Vector &storageVector)
		{
			T *newObject = nullptr;
			if constexpr (std::is_void_v<typename Traits::DerType>)
			{
				newObject = new T(*this);
			}
			else
			{
				newObject = new T(static_cast<typename Traits::DerType &>(*this));
			}
			storageVector.emplace_back(newObject);
			return newObject;
		}

		template <typename T, typename Key, typename Map>
		T *findObject(const Key &key, Map &helperMap)
		{
			auto it = helperMap.find(key);
			return (it != helperMap.end()) ? it->second : nullptr;
		}

		template <typename T, typename Key, typename Map, typename Vector>
		T *findOrCreateObject(const Key &key, Map &helperMap, Vector &storageVector, bool *outWasCreated = nullptr)
		{
			auto it = helperMap.find(key);
			if (it != helperMap.end())
			{
				if (outWasCreated)
					*outWasCreated = false;
				return it->second;
			}
			T *newObject = nullptr;

			if constexpr (std::is_void_v<typename Traits::DerType>)
			{
				newObject = new T(*this);
			}
			else
			{
				newObject = new T(static_cast<typename Traits::DerType &>(*this));
			}
			helperMap[key] = newObject;
			storageVector.emplace_back(newObject);
			if (outWasCreated)
				*outWasCreated = true;
			return newObject;
		}

		void build(const std::vector<std::vector<int>> &polygons,
				   const std::vector<glm::vec3> &vertices)
		{
			// Clear old data (important if reused)
			m_halfEdges.clear();
			m_vertices.clear();
			m_edges.clear();
			m_faces.clear();

			// --- HELPERS ---
			std::map<int, typename Traits::VType *> vertexMap;
			std::map<std::pair<int, int>, typename Traits::HType *> halfEdgeMap;

			// --- CREATE VERTICES ---
			for (int i = 0; i < vertices.size(); ++i)
			{
				typename Traits::VType *v = createObject<typename Traits::VType>(m_vertices);
				v->m_vertexIndexInVector = m_vertices.size() - 1;
				v->m_position = vertices[i];

				vertexMap[i] = v;
			}

			// --- PROCESS FACES ---
			for (const auto &polygon : polygons)
			{
				if (polygon.size() < 3)
					continue;

				typename Traits::FType *face = createObject<typename Traits::FType>(m_faces);
				face->m_faceIndexInVector = m_faces.size() - 1;

				std::vector<typename Traits::HType *> faceHalfEdges;

				int n = polygon.size();

				// --- CREATE HALF-EDGES ---
				for (int i = 0; i < n; ++i)
				{
					int v0 = polygon[i];
					int v1 = polygon[(i + 1) % n];

					typename Traits::HType *he = createObject<typename Traits::HType>(m_halfEdges);
					he->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

					he->m_vertex = vertexMap[v0];
					he->m_face = face;

					// Assign vertex outgoing half-edge if not set
					if (!vertexMap[v0]->m_halfEdge)
						vertexMap[v0]->m_halfEdge = he;

					faceHalfEdges.push_back(he);

					// --- TWIN DETECTION ---
					auto twinIt = halfEdgeMap.find({v1, v0});
					if (twinIt != halfEdgeMap.end())
					{
						he->m_twin = twinIt->second;
						twinIt->second->m_twin = he;

						he->m_edge = twinIt->second->m_edge;
						he->m_edge->m_isOuter = false;
					}
					else
					{
						typename Traits::EType *edge = createObject<typename Traits::EType>(m_edges);
						edge->m_edgeIndexInVector = m_edges.size() - 1;

						edge->m_halfEdge = he;
						edge->m_firstVertex = vertexMap[v0];
						edge->m_secondVertex = vertexMap[v1];
						edge->m_isOuter = true;

						he->m_edge = edge;

						halfEdgeMap[{v0, v1}] = he;
					}
				}

				// --- LINK NEXT / PREVIOUS ---
				for (int i = 0; i < n; ++i)
				{
					typename Traits::HType *current = faceHalfEdges[i];
					typename Traits::HType *next = faceHalfEdges[(i + 1) % n];
					typename Traits::HType *prev = faceHalfEdges[(i - 1 + n) % n];

					current->m_next = next;
					current->m_previous = prev;
				}

				// --- ASSIGN FACE HALF-EDGE ---
				face->m_halfEdge = faceHalfEdges[0];
			}
		}

	public:
		std::vector<typename Traits::HType *> m_halfEdges;
		std::vector<typename Traits::VType *> m_vertices;
		std::vector<typename Traits::EType *> m_edges;
		std::vector<typename Traits::FType *> m_faces;
	};
}
