#pragma once
#include "Material.h"
#include "DataStructures/HalfEdge.h"
#include "Renderer/VertexDataStructs.h"

/*
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/mark_domain_in_triangulation.h>
#include <unordered_map>
#include <CGAL/partition_2.h>
#include <cassert>
#include <list>
#include <random>
*/

//CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/mark_domain_in_triangulation.h>
#include <CGAL/draw_triangulation_2.h>
//


#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include "RayIntersectionAlg.h"


/*
namespace Eigen {
	// Define traits for glm::vec3
	template<> struct NumTraits<glm::vec3> : NumTraits<float> {
		typedef glm::vec3 Real;
		typedef glm::vec3 NonInteger;
		typedef glm::vec3 Nested;
		enum {
			IsComplex = 0,
			IsInteger = 0,
			IsSigned = 1,
			RequireInitialization = 1,
			ReadCost = 1,
			AddCost = 1,
			MulCost = 1
		};
	};
}
*/

//helper structs
struct Element
{
	Material* material;
	size_t index;
	bool deleted;
};

struct MovedElement
{
	Material* material;
	size_t index;
	size_t newIndexInMaterialVector;
};

struct StartEndOfElement
{
	size_t start;
	size_t end;
};

struct StartEndMovedElements
{
	std::vector<StartEndOfElement> startEndOfElements;
	std::vector<MovedElement> movedElements;
};
//

struct VectorData
{
	std::vector<Element> m_materialIDIndexMap;
	//prvy vektor indexom odkazuje na vektor pod nim
	std::vector<MeshVert> m_vertexData;
};

struct FaceData
{
	size_t faceIndex;
	size_t startIndex;
	size_t endIndex;
	glm::vec3 normal;
};

enum AngleType
{
	CONVEX = 0,
	CONCAVE = 1,
	FLAT = 2
};

enum PolygonOrientation
{
	CLOCKWISE = 0,
	COUNTERCLOCKWISE = 1
};

/*
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_tag                               Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, CGAL::Default, Itag> CDT;
typedef CDT::Face_handle                                          Face_handle;
typedef CGAL::Partition_traits_2<K>                         Traits;
typedef Traits::Polygon_2                                   Polygon_2;
*/

//CGAL
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Point Point;
//



//least squares CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/linear_least_squares_fitting_3.h>
#include <iostream>
#include <vector>

//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_3 Point_3;
typedef K::Plane_3 Plane_3;
typedef std::vector<Point_3> PointList;
//

#include "GeometryAlgorithms/TriangleTriangulator.h"
#include "GeometryAlgorithms/NgonPlanarTriangulator.h"

/*
struct MeshRenderingData
{
	std::vector<LineVertex> m_edges;
	std::vector<MeshPoint> m_points;
};
*/

class Mesh
{
public:
	//chceme sem pridat mapovanie materialov k jednotlivym vaos
	HalfEdgeDS::HalfEdgeMesh* m_halfEdgeMesh = nullptr;
	//std::vector<TestFace*> m_selectedFaces;
	//std::vector<HalfEdgeDS::Face*> m_selectedFaces;

	//co musi mat octree, je samozrejme Face*, taktiez ale aj start a end indexy


	std::map<Material*, VectorData> m_materialIDVertexDataMap;

	VectorData m_combinedVertexDataMatVector;

	std::string m_meshID;
	Material* m_defaultMaterial;

	std::vector<FaceData> m_facesData;

	PolygonTriangulator* m_triangleTriangulator;
	PolygonTriangulator* m_polygonTriangulator;

	//MeshRenderingData m_meshRenderingData;

	bool m_buildSuccessful = false;
public:

	Mesh(Material* defaultMaterial , std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices, const std::vector<int>& polygonNormalIndices = std::vector<int>(), const std::vector<glm::vec3>& polygonsNormals = std::vector<glm::vec3>())
	{
		m_defaultMaterial = defaultMaterial;
		m_halfEdgeMesh = new HalfEdgeDS::HalfEdgeMesh();
		m_triangleTriangulator = new TriangleTriangulator();
		m_polygonTriangulator = new NgonPlanarTriangulator();

		m_halfEdgeMesh->build(polygonsIndices, polygonsVertices);
		m_buildSuccessful = buildMeshData(polygonsIndices, polygonsVertices, polygonNormalIndices, polygonsNormals);


		//setRenderingData();
	}

	bool isBuildSuccessful()
	{
		return m_buildSuccessful;
	}

	bool buildMeshData(std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices, const std::vector<int>& polygonNormalIndices = std::vector<int>(), const std::vector<glm::vec3>& polygonsNormals = std::vector<glm::vec3>())
	{
		size_t startIndexOfTrianglesInFace = 0;
		size_t endIndexOfTrianglesInFace = 0;
		size_t vertexIndex = 0;
		size_t currentFaceIndex = 0;
		const auto& materialIDVertexDataIt = m_materialIDVertexDataMap.insert({ m_defaultMaterial, VectorData() });

		bool triangulationSuccess;
		for (const auto& polygonIndices : polygonsIndices)
		{
			std::vector<glm::vec3> triangulatedVerticesOfPolygon;
			glm::vec3 polygonNormal;

			if(!m_triangleTriangulator->calculateTriangulation(polygonIndices, polygonsVertices, triangulatedVerticesOfPolygon, polygonNormal))
			{
				triangulationSuccess = false;

				/*
				if(!m_polygonTriangulator->calculateTriangulation(polygonIndices, polygonsVertices, triangulatedVerticesOfPolygon, polygonNormal))
				{
					triangulationSuccess = false;
				} else
				{
					triangulationSuccess = true;
				}
				*/
			} else
			{
				triangulationSuccess = true;
			}

			if(triangulationSuccess)
			{
				if (!polygonsNormals.empty() && !polygonNormalIndices.empty()) {

					polygonNormal = polygonsNormals.at(polygonNormalIndices.at(currentFaceIndex));
				}

				// Update end index for the next iteration
				endIndexOfTrianglesInFace = startIndexOfTrianglesInFace + triangulatedVerticesOfPolygon.size() - 1;

				//mame k dispozicii triangulatedVertices daneho facu
				for (const auto& triangulatedVertexOfPolygon : triangulatedVerticesOfPolygon)
				{
					setInitialDataForVertex(m_combinedVertexDataMatVector, materialIDVertexDataIt.first->second, { m_defaultMaterial, vertexIndex, false }, triangulatedVertexOfPolygon, polygonNormal);
					++vertexIndex;
				}
				m_facesData.push_back({ currentFaceIndex, startIndexOfTrianglesInFace, endIndexOfTrianglesInFace, polygonNormal });
				++currentFaceIndex;
				// Update start index to start from the next index
				startIndexOfTrianglesInFace = endIndexOfTrianglesInFace + 1;
			} else
			{
				//triangulacia nebola uspesna
				return triangulationSuccess;
			}

		}
		return triangulationSuccess;
	}

	void setInitialDataForVertex(VectorData& combinedVertexDataMat, VectorData& materialVertexDataMat, const Element& elementData, const glm::vec3& vertexAfterTriangulation, const glm::vec3& polygonNormal)
	{
		combinedVertexDataMat.m_materialIDIndexMap.push_back({ elementData.material, elementData.index, elementData.deleted });
		combinedVertexDataMat.m_vertexData.push_back({ vertexAfterTriangulation, polygonNormal});

		materialVertexDataMat.m_materialIDIndexMap.push_back({ elementData.material, elementData.index, elementData.deleted });
		materialVertexDataMat.m_vertexData.push_back({ vertexAfterTriangulation, polygonNormal });
	}


	float calculateSignedAreaOf2DPolygon(const std::vector<glm::vec2>& vertices) {
		float area = 0.0;
		size_t j = 1;
		for (size_t i = 0; i < vertices.size(); i++, j++) {
			j = j % vertices.size();
			area += (vertices.at(j).x - vertices.at(i).x) * (vertices.at(j).y + vertices.at(i).y);
		}
		return area / 2.0;
	}

	bool arePointsCoplanar(const PointList& points, double threshold = 1e-6) {
		// Compute the best-fitting plane
		Plane_3 plane;
		linear_least_squares_fitting_3(points.begin(), points.end(),
			plane, CGAL::Dimension_tag<0>());

		// Compute the normal vector of the fitted plane
		auto normal = plane.orthogonal_vector();

		// Get the residuals (errors) from the fitting process
		std::vector<double> residuals;
		residuals.reserve(points.size());
		for (const auto& point : points) {
			// Compute the signed distance of the point to the plane
			double distance = (point - plane.point()) * normal; // Dot product
			residuals.push_back(std::abs(distance));
		}

		// Compute the mean squared error
		double meanSquaredError = 0.0;
		for (const auto& residual : residuals) {
			meanSquaredError += residual * residual;
		}
		meanSquaredError /= residuals.size();

		// Check if the mean squared error is below the threshold
		return meanSquaredError <= threshold;
	}



	std::tuple<Eigen::Vector3d, Eigen::Vector3d, double> calculateLeastSquaresFittingPlane(const std::vector<Eigen::Vector3d>& points) {
		// Calculate centroid
		Eigen::Vector3d centroid = Eigen::Vector3d::Zero();
		for (const auto& point : points) {
			centroid += point;
		}
		centroid /= points.size();

		// Construct covariance matrix
		Eigen::Matrix3d covariance_matrix = Eigen::Matrix3d::Zero();
		for (const auto& point : points) {
			covariance_matrix += (point - centroid) * (point - centroid).transpose();
		}
		covariance_matrix /= points.size();

		
		Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eigensolver(covariance_matrix);
		Eigen::Vector3d normal = eigensolver.eigenvectors().col(0); // Eigenvector corresponding to smallest eigenvalue

		
		if (normal.dot(centroid) > 0) {
			normal *= -1;
		}

		// Compute error
		double error = 0.0;
		for (const auto& point : points) {
			double distance = std::abs((point - centroid).dot(normal)); // Signed distance to the plane
			error += distance * distance;
		}
		error /= points.size();
		
		return std::make_tuple(centroid, normal, error);
	}

	void triangulate2DPolygon(std::vector<int>& polygon, const std::vector<glm::vec2>& vertices, std::vector<glm::vec2>& accumulatedPointsData)
	{
		//musime vytvorit std::vector<Vertex_handle> vertices
		CDT cdt;
		std::vector<Vertex_handle> vertsInVertexHandle;

		for (const auto& vertex : vertices)
		{
			vertsInVertexHandle.push_back(cdt.insert(Point(vertex.x, vertex.y)));
		}
		
		//teraz ist cez edges
		for (int i = 0; i < polygon.size(); ++i)
		{
			cdt.insert_constraint(vertsInVertexHandle.at(polygon.at(i)), vertsInVertexHandle.at(polygon.at((i + 1) % polygon.size())));
		}


		CGAL::mark_domain_in_triangulation(cdt);
		CGAL::refine_Delaunay_mesh_2(cdt,
			CGAL::parameters::domain_is_initialized(true));
		for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin();
			fit != cdt.finite_faces_end(); ++fit)
		{
			if (fit->is_in_domain()) {
				accumulatedPointsData.push_back({ fit->vertex(0)->point().x(), fit->vertex(0)->point().y() });
				accumulatedPointsData.push_back({ fit->vertex(1)->point().x(), fit->vertex(1)->point().y() });
				accumulatedPointsData.push_back({ fit->vertex(2)->point().x(), fit->vertex(2)->point().y() });
			}
		}
	}


	float angleBetweenVectors(const glm::vec3& a, const glm::vec3& b) {
		float dotProduct = glm::dot(a, b);

		// Calculate the magnitudes of the vectors
		float magnitudeA = glm::length(a);
		float magnitudeB = glm::length(b);

		float angleRadians = std::acos(dotProduct / (magnitudeA * magnitudeB));

		float angleDegrees = glm::degrees(angleRadians);
		return angleDegrees;
	}

	glm::vec3 reflectVector(const glm::vec3& vector, const glm::vec3& normal)
	{
		glm::vec3 reflectedVector = vector - 2.0f * glm::dot(vector, normal) * normal;
		return reflectedVector;
	}

	void calculateAngleTypesOfVertices(const std::vector<int>& polygon, const PolygonOrientation& polygonOrientation, const std::vector<glm::vec3>& vertices, const glm::vec3& polygonNormal, std::map<int, AngleType>& returnedAngleTypes)
	{
		for (int i = 0; i < polygon.size(); ++i)
		{
			//potrebujem ziskat vzdy predosly vertex
			//pre COUNTERCLOCKWISE
			glm::vec3 previousVertex = vertices.at(polygon.at((i + (polygon.size() - 1)) % polygon.size()));
			glm::vec3 currentVertex = vertices.at(polygon.at(i % polygon.size()));
			glm::vec3 nextVertex = vertices.at(polygon.at((i + 1) % polygon.size()));

			std::cout << "PreviousVertex: " << " X: " << previousVertex.x << ", Y: " << previousVertex.y << ", Z: " << previousVertex.z << std::endl;
			std::cout << "CurrentVertex: " << " X: " << currentVertex.x << ", Y: " << currentVertex.y << ", Z: " << currentVertex.z << std::endl;
			std::cout << "NextVertex: " << " X: " << nextVertex.x << ", Y: " << nextVertex.y << ", Z: " << nextVertex.z << std::endl;

			glm::vec3 vector_prev_cur = currentVertex - previousVertex;
			glm::vec3 vector_prev_next = nextVertex - previousVertex;

			glm::vec2 test1;
			glm::vec2 test2;

			const auto& dotProduct = glm::dot(glm::cross(vector_prev_cur, vector_prev_next), polygonNormal);


			if (polygonOrientation == COUNTERCLOCKWISE)
			{
				//COUNTERCLOCKWISE
				if (dotProduct < 0.0)
				{
					returnedAngleTypes[polygon.at(i)] = AngleType::CONCAVE;
					//viac ako 180 stupnov
				}
				else if (dotProduct > 0.0)
				{
					returnedAngleTypes[polygon.at(i)] = AngleType::CONVEX;
					//menej ako 180 stupnov
				} else
				{
					returnedAngleTypes[polygon.at(i)] = AngleType::FLAT;
				}
			}
			else
			{
				//CLOCKWISE
				if (dotProduct < 0.0)
				{
					returnedAngleTypes[polygon.at(i)] = AngleType::CONCAVE;
					//viac ako 180 stupnov

				}
				else if (dotProduct > 0.0)
				{
					returnedAngleTypes[polygon.at(i)] = AngleType::CONVEX;
					//menej ako 180 stupnov
				} else
				{
					returnedAngleTypes[polygon.at(i)] = AngleType::FLAT;
				}
			}

		}
	}


	bool calculatePolygonNormal(const std::vector<int>& polygon, std::map<int, AngleType>& angleTypesMap, const PolygonOrientation& polygonOrientation, const std::vector<glm::vec3>& vertices, glm::vec3& returnedNormal) {
		bool calculatedNormal = false;
		for (int i = 0; i < polygon.size(); ++i)
		{
			const auto& polygonVertexPrev = polygon.at((i + (polygon.size() - 1)) % polygon.size());
			const auto& polygonVertexCur = polygon.at(i % polygon.size());
			const auto& polygonVertexNext = polygon.at((i + 1) % polygon.size());

			glm::vec3 previousVertex = vertices.at(polygonVertexPrev);
			glm::vec3 currentVertex = vertices.at(polygonVertexCur);
			glm::vec3 nextVertex = vertices.at(polygonVertexNext);

			glm::vec3 vector_cur_prev = previousVertex - currentVertex;
			glm::vec3 vector_cur_next = nextVertex - currentVertex;

			const auto& angleTypesIt = angleTypesMap.find(polygonVertexCur);

			if(polygonOrientation == COUNTERCLOCKWISE)
			{
				if(angleTypesIt != angleTypesMap.end())
				{
					//nasli sme ho
					if (angleTypesIt->second == AngleType::CONCAVE)
					{
						returnedNormal = glm::cross(vector_cur_prev, vector_cur_next);
						calculatedNormal = true;
					}
					else if (angleTypesIt->second == AngleType::CONVEX)
					{
						returnedNormal = glm::cross(vector_cur_next, vector_cur_prev);
						calculatedNormal = true;
					}
				}

			} else
			{
				//CLOCKWISE

				if(angleTypesIt->second == AngleType::CONVEX)
				{
					std::cout << "CONVEX" << std::endl;
					returnedNormal = glm::cross(vector_cur_next, vector_cur_prev);
					calculatedNormal = true;
				} else if (angleTypesIt->second == AngleType::CONCAVE)
				{
					std::cout << "CONCAVE" << std::endl;
					returnedNormal = glm::cross(vector_cur_prev, vector_cur_next);
					calculatedNormal = true;
				}
			}

			if (calculatedNormal)
			{
				returnedNormal = glm::normalize(returnedNormal);
				return true;
			}

		}
		return false;
	}

	void triangulateTriangle()
	{
		
	}

	bool buildMeshData(std::vector<std::vector<int>>& polygons, const std::vector<glm::vec3>& vertices)
	{
		size_t startIndexOfTrianglesInFace = 0;
		size_t endIndexOfTrianglesInFace = 0;

		size_t vertexIndex = 0;

		size_t currentFaceIndex = 0;

		//pre-add one default materialVector
		//getThatMaterialVector
		const auto& materialIDVertexDataIt = m_materialIDVertexDataMap.insert({ m_defaultMaterial, VectorData() });

		std::vector<glm::vec2> projectedVerticesOnZPlane;
		std::vector<glm::vec2> projectedVerticesOnYPlane;
		std::vector<glm::vec2> projectedVerticesOnXPlane;

		std::vector<glm::vec3> projectedVerticesOnZPlane3D;
		std::vector<glm::vec3> projectedVerticesOnYPlane3D;
		std::vector<glm::vec3> projectedVerticesOnXPlane3D;

		for (const auto& vertex : vertices)
		{
			projectedVerticesOnZPlane.push_back({ vertex.x, vertex.y });
			projectedVerticesOnYPlane.push_back({ vertex.x, vertex.z });
			projectedVerticesOnXPlane.push_back({ vertex.y, vertex.z });

			projectedVerticesOnZPlane3D.push_back({vertex.x, vertex.y, 0.0});
			projectedVerticesOnYPlane3D.push_back({vertex.x, 0.0, vertex.z});
			projectedVerticesOnXPlane3D.push_back({ 0.0, vertex.y, vertex.z });
		}
		int currentPolygonIndex = 0;
		for (auto& polygon : polygons)
		{
			//predtym skontrolovat ci je polygon vôbec planarny
			std::vector<Eigen::Vector3d> polygonVertices;

			std::vector<glm::vec2> projected2DpointsOfPolygonOnZPlane;
			std::vector<glm::vec2> projected2DpointsOfPolygonOnYPlane;
			std::vector<glm::vec2> projected2DpointsOfPolygonOnXPlane;

			for (int polygonPointIndex = 0; polygonPointIndex < polygon.size(); ++polygonPointIndex)
			{

				polygonVertices.push_back({ vertices.at(polygon.at(polygonPointIndex)).x, vertices.at(polygon.at(polygonPointIndex)).y, vertices.at(polygon.at(polygonPointIndex)).z });

				projected2DpointsOfPolygonOnZPlane.emplace_back(vertices.at(polygon.at(polygonPointIndex)).x, vertices.at(polygon.at(polygonPointIndex)).y);
				projected2DpointsOfPolygonOnYPlane.emplace_back(vertices.at(polygon.at(polygonPointIndex)).x, vertices.at(polygon.at(polygonPointIndex)).z);
				projected2DpointsOfPolygonOnXPlane.emplace_back(vertices.at(polygon.at(polygonPointIndex)).y, vertices.at(polygon.at(polygonPointIndex)).z);

			}

			

			//skontroluj planaritu
			const auto& planarityResult = calculateLeastSquaresFittingPlane(polygonVertices);

			//glm::vec3 polygonNormal;
			//calculatePolygonNormal(polygon, polygonsOrientation.at(currentPolygonIndex), vertices, polygonNormal);
			//uistime sa ze projektnuty plane a naš polygon budu mat rovnaku normalu


			//std::cout << "Polygon normal: " << " X: " << polygonNormal.x << ", Y: " << polygonNormal.y << ", Z:" << polygonNormal.z << std::endl;

			if (std::get<2>(planarityResult) < 0.000001)
			{
				//polygon je planarny
				//mame normalu a bod
				glm::vec3 normalOfPolygonPlane = glm::vec3(std::get<1>(planarityResult).x(), std::get<1>(planarityResult).y(), std::get<1>(planarityResult).z());
				glm::vec3 centroid = glm::vec3(std::get<0>(planarityResult).x(), std::get<0>(planarityResult).y(), std::get<0>(planarityResult).z());

				float areaOfPolygonProjectedOnZPlane = calculateSignedAreaOf2DPolygon(projected2DpointsOfPolygonOnZPlane);
				float areaOfPolygonProjectedOnYPlane = calculateSignedAreaOf2DPolygon(projected2DpointsOfPolygonOnYPlane);
				float areaOfPolygonProjectedOnXPlane = calculateSignedAreaOf2DPolygon(projected2DpointsOfPolygonOnXPlane);

				std::cout << "AreaOfPolygonProjectedOnZPlane: " << areaOfPolygonProjectedOnZPlane << std::endl;
				std::cout << "AreaOfPolygonProjectedOnYPlane: " << areaOfPolygonProjectedOnYPlane << std::endl;
				std::cout << "AreaOfPolygonProjectedOnXPlane: " << areaOfPolygonProjectedOnXPlane << std::endl;


				std::vector<glm::vec2> vertexDataOfProjectedPolygon;
				if (glm::abs(areaOfPolygonProjectedOnZPlane) > 0.0001)
				{
					std::cout << "Projected on Z Plane" << std::endl;

					PolygonOrientation polygonOrientation;
					glm::vec3 projectedZNormal;

					if(areaOfPolygonProjectedOnZPlane < 0.0)
					{
						polygonOrientation = PolygonOrientation::COUNTERCLOCKWISE;
						projectedZNormal = glm::vec3(0.0, 0.0, 1.0);
					} else if (areaOfPolygonProjectedOnZPlane > 0.0)
					{
						polygonOrientation = PolygonOrientation::CLOCKWISE;
						projectedZNormal = glm::vec3(0.0, 0.0, -1.0);
					}

					
					

					//potrebujem si zapisat pre vsetky vertexy k nim ich uhol ktory zovieraju
					std::map<int, AngleType> angleTypesOfProjectedVerticesOnZPlane;
					calculateAngleTypesOfVertices(polygon, polygonOrientation, projectedVerticesOnZPlane3D, projectedZNormal, angleTypesOfProjectedVerticesOnZPlane);
					glm::vec3 polygonNormal;
					calculatePolygonNormal(polygon, angleTypesOfProjectedVerticesOnZPlane, polygonOrientation, vertices, polygonNormal);


					//tak pootom pouzi ZPlane ako projekciu
					if(polygon.size() > 3)
					{
						triangulate2DPolygon(polygon, projectedVerticesOnZPlane, vertexDataOfProjectedPolygon);
					} else
					{
						vertexDataOfProjectedPolygon = projected2DpointsOfPolygonOnZPlane;
					}
					// Update end index for the next iteration
					endIndexOfTrianglesInFace = startIndexOfTrianglesInFace + vertexDataOfProjectedPolygon.size() - 1;

					for (const auto& projectedVertex : vertexDataOfProjectedPolygon)
					{
						std::cout << "ProjectedVertex: " << " X: " << projectedVertex.x << ", Y: " << projectedVertex.y << std::endl;
						//bod
						glm::vec3 pointOfProjectedVertex(projectedVertex.x, projectedVertex.y, 0.0);
						//smer
						glm::vec3 directionOfProjectedVertex(0.0, 0.0, 1.0);
						float amountToBeMultiplied = 0.0;


						if(!RayIntersectionAlg::intersectPlane(polygonNormal, centroid, pointOfProjectedVertex, directionOfProjectedVertex, amountToBeMultiplied))
						{
							directionOfProjectedVertex.z = -1.0;
							if(RayIntersectionAlg::intersectPlane(polygonNormal, centroid, pointOfProjectedVertex, directionOfProjectedVertex, amountToBeMultiplied))
							{
								std::cout << "Plane intersects" << std::endl;
							} else
							{
								std::cout << "Plane doesn't intersects" << std::endl;
							}
						} else
						{
							std::cout << "Plane intersects" << std::endl;
						}

						float zComponentAfterTriangulation = directionOfProjectedVertex.z * amountToBeMultiplied;
						if (glm::abs(zComponentAfterTriangulation) < 0.000001)
						{
							zComponentAfterTriangulation = 0.0;
						}

						glm::vec3 vertexAfterTriangulation(projectedVertex.x,projectedVertex.y, zComponentAfterTriangulation);

						std::cout << "VertexAfterTriangulation: " << " X: " << vertexAfterTriangulation.x << ", Y: " << vertexAfterTriangulation.y << ", Z:" << vertexAfterTriangulation.z << std::endl;


						m_combinedVertexDataMatVector.m_materialIDIndexMap.push_back({ m_defaultMaterial, vertexIndex, false });
						m_combinedVertexDataMatVector.m_vertexData.push_back({ {vertexAfterTriangulation.x, vertexAfterTriangulation.y, vertexAfterTriangulation.z},{polygonNormal} });

						materialIDVertexDataIt.first->second.m_materialIDIndexMap.push_back({ m_defaultMaterial, vertexIndex, false });
						materialIDVertexDataIt.first->second.m_vertexData.push_back({ {vertexAfterTriangulation.x, vertexAfterTriangulation.y, vertexAfterTriangulation.z},{polygonNormal} });

						++vertexIndex;
					}
					//teraz musim facom priradit start a end indexy
					m_facesData.push_back({ currentFaceIndex, startIndexOfTrianglesInFace, endIndexOfTrianglesInFace, polygonNormal });
					++currentFaceIndex;
					// Update start index to start from the next index
					startIndexOfTrianglesInFace = endIndexOfTrianglesInFace + 1;
				}
				else if (glm::abs(areaOfPolygonProjectedOnYPlane) > 0.0001)
				{
					std::cout << "Projected on Y Plane" << std::endl;
					//tak pootom pouzi YPlane ako projekciu


					PolygonOrientation polygonOrientation;
					glm::vec3 projectedYNormal;

					if (areaOfPolygonProjectedOnYPlane < 0.0)
					{
						polygonOrientation = PolygonOrientation::COUNTERCLOCKWISE;
						projectedYNormal = glm::vec3(0.0, -1.0, 0.0);
					}
					else if (areaOfPolygonProjectedOnYPlane > 0.0)
					{
						polygonOrientation = PolygonOrientation::CLOCKWISE;
						projectedYNormal = glm::vec3(0.0, 1.0, 0.0);
					}


					std::map<int, AngleType> angleTypesOfProjectedVerticesOnYPlane;
					calculateAngleTypesOfVertices(polygon, polygonOrientation, projectedVerticesOnYPlane3D, projectedYNormal, angleTypesOfProjectedVerticesOnYPlane);
					glm::vec3 polygonNormal;
					calculatePolygonNormal(polygon, angleTypesOfProjectedVerticesOnYPlane, polygonOrientation, vertices, polygonNormal);

					//tak pootom pouzi ZPlane ako projekciu
					if (polygon.size() > 3)
					{
						triangulate2DPolygon(polygon, projectedVerticesOnYPlane, vertexDataOfProjectedPolygon);
					}
					else
					{
						vertexDataOfProjectedPolygon = projected2DpointsOfPolygonOnYPlane;
					}

					// Update end index for the next iteration
					endIndexOfTrianglesInFace = startIndexOfTrianglesInFace + vertexDataOfProjectedPolygon.size() - 1;


					//triangulate2DPolygon(polygon, projectedVerticesOnYPlane, vertexDataOfProjectedPolygon);
					//startIndexOfTrianglesInFace = endIndexOfTrianglesInFace;
					//endIndexOfTrianglesInFace += vertexDataOfProjectedPolygon.size() - 1;

					for (const auto& projectedVertex : vertexDataOfProjectedPolygon)
					{
						//bod
						glm::vec3 pointOfProjectedVertex(projectedVertex.x, 0.0, projectedVertex.y);
						//smer
						glm::vec3 directionOfProjectedVertex(0.0, 1.0, 0.0);
						float amountToBeMultiplied = 0.0;


						if (!RayIntersectionAlg::intersectPlane(polygonNormal, centroid, pointOfProjectedVertex, directionOfProjectedVertex, amountToBeMultiplied))
						{
							directionOfProjectedVertex.y = -1.0;
							if(RayIntersectionAlg::intersectPlane(polygonNormal, centroid, pointOfProjectedVertex, directionOfProjectedVertex, amountToBeMultiplied))
							{
								std::cout << "Plane intersects" << std::endl;
							} else
							{
								std::cout << "Plane does not intersects" << std::endl;
							}
						} else
						{
							std::cout << "Plane intersects" << std::endl;
						}
						float yComponentAfterTriangulation = directionOfProjectedVertex.y * amountToBeMultiplied;
						if(glm::abs(yComponentAfterTriangulation) < 0.000001)
						{
							yComponentAfterTriangulation = 0.0;
						}
						glm::vec3 vertexAfterTriangulation(projectedVertex.x, yComponentAfterTriangulation, projectedVertex.y);

						m_combinedVertexDataMatVector.m_materialIDIndexMap.push_back({ m_defaultMaterial, vertexIndex, false });
						m_combinedVertexDataMatVector.m_vertexData.push_back({ {vertexAfterTriangulation.x, vertexAfterTriangulation.y, vertexAfterTriangulation.z},{polygonNormal} });

						materialIDVertexDataIt.first->second.m_materialIDIndexMap.push_back({ m_defaultMaterial, vertexIndex, false });
						materialIDVertexDataIt.first->second.m_vertexData.push_back({ {vertexAfterTriangulation.x, vertexAfterTriangulation.y, vertexAfterTriangulation.z},{polygonNormal} });

						++vertexIndex;
					}
					//teraz musim facom priradit start a end indexy
					m_facesData.push_back({ currentFaceIndex, startIndexOfTrianglesInFace, endIndexOfTrianglesInFace, polygonNormal });
					++currentFaceIndex;
					// Update start index to start from the next index
					startIndexOfTrianglesInFace = endIndexOfTrianglesInFace + 1;

				}
				else if (glm::abs(areaOfPolygonProjectedOnXPlane) > 0.0001)
				{
					std::cout << "Projected on X Plane" << std::endl;
					//tak pootom pouzi XPlane ako projekciu


					PolygonOrientation polygonOrientation;
					glm::vec3 projectedXNormal;

					if (areaOfPolygonProjectedOnXPlane < 0.0)
					{
						polygonOrientation = PolygonOrientation::COUNTERCLOCKWISE;
						projectedXNormal = glm::vec3(1.0, 0.0, 0.0);
					}
					else if (areaOfPolygonProjectedOnYPlane > 0.0)
					{
						polygonOrientation = PolygonOrientation::CLOCKWISE;
						projectedXNormal = glm::vec3(-1.0, 0.0, 0.0);
					}


					std::map<int, AngleType> angleTypesOfProjectedVerticesOnXPlane;
					calculateAngleTypesOfVertices(polygon, polygonOrientation, projectedVerticesOnXPlane3D, projectedXNormal, angleTypesOfProjectedVerticesOnXPlane);
					glm::vec3 polygonNormal;
					calculatePolygonNormal(polygon, angleTypesOfProjectedVerticesOnXPlane, polygonOrientation, vertices, polygonNormal);


					//tak pootom pouzi ZPlane ako projekciu
					if (polygon.size() > 3)
					{
						triangulate2DPolygon(polygon, projectedVerticesOnXPlane, vertexDataOfProjectedPolygon);
					}
					else
					{
						vertexDataOfProjectedPolygon = projected2DpointsOfPolygonOnXPlane;
					}
					startIndexOfTrianglesInFace = endIndexOfTrianglesInFace;
					endIndexOfTrianglesInFace += vertexDataOfProjectedPolygon.size();


					//triangulate2DPolygon(polygon, projectedVerticesOnXPlane, vertexDataOfProjectedPolygon);
					//startIndexOfTrianglesInFace = endIndexOfTrianglesInFace;
					//endIndexOfTrianglesInFace += vertexDataOfProjectedPolygon.size() - 1;

					for (const auto& projectedVertex : vertexDataOfProjectedPolygon)
					{
						//bod
						glm::vec3 pointOfProjectedVertex(0.0, projectedVertex.x, projectedVertex.y);
						//smer
						glm::vec3 directionOfProjectedVertex(1.0, 0.0, 0.0);
						float amountToBeMultiplied = 0.0;

						if (!RayIntersectionAlg::intersectPlane(polygonNormal, centroid, pointOfProjectedVertex, directionOfProjectedVertex, amountToBeMultiplied))
						{
							directionOfProjectedVertex.x = -1.0;
							RayIntersectionAlg::intersectPlane(polygonNormal, centroid, pointOfProjectedVertex, directionOfProjectedVertex, amountToBeMultiplied);
						}

						float xComponentAfterTriangulation = directionOfProjectedVertex.x * amountToBeMultiplied;
						if (glm::abs(xComponentAfterTriangulation) < 0.000001)
						{
							xComponentAfterTriangulation = 0.0;
						}
						glm::vec3 vertexAfterTriangulation(xComponentAfterTriangulation, projectedVertex.x, projectedVertex.y);

						m_combinedVertexDataMatVector.m_materialIDIndexMap.push_back({ m_defaultMaterial, vertexIndex, false });
						m_combinedVertexDataMatVector.m_vertexData.push_back({ {vertexAfterTriangulation.x, vertexAfterTriangulation.y, vertexAfterTriangulation.z},{polygonNormal} });

						materialIDVertexDataIt.first->second.m_materialIDIndexMap.push_back({ m_defaultMaterial, vertexIndex, false });
						materialIDVertexDataIt.first->second.m_vertexData.push_back({ {vertexAfterTriangulation.x, vertexAfterTriangulation.y, vertexAfterTriangulation.z},{polygonNormal} });

						++vertexIndex;
					}
					//teraz musim facom priradit start a end indexy
					m_facesData.push_back({ currentFaceIndex, startIndexOfTrianglesInFace, endIndexOfTrianglesInFace, polygonNormal });
					++currentFaceIndex;
					// Update start index to start from the next index
					startIndexOfTrianglesInFace = endIndexOfTrianglesInFace + 1;
				}

			} else{
				//polygon je neplanarny
				return false;
			}
			++currentPolygonIndex;
		}
		return true;
	}

	void assignMaterialToMesh(Material* material)
	{
		
	}

	void assignMaterialToSelectedFaces(Material* material, std::vector<HalfEdgeDS::Face*>& selectedFaces)
	{
		std::map<Material*, StartEndMovedElements> startEndMovedElementsMap;

		//najskor budeme prechadzat cez vsetky faces

		for (const auto& selectedFace : selectedFaces)
		{
			//no a teraz sa musim pozriet na to ci vkladany material
			//uz existuje v nasej mape
			//a taktiez vkladany material je rozdielny od toho co je na combinedVectore

			size_t faceIndex = std::distance(selectedFace->getMesh().faceIterBegin(), selectedFace->getHalfEdge()->getFace());
			size_t startFaceVertIndex = m_facesData.at(faceIndex).startIndex;
			size_t endFaceVertIndex = m_facesData.at(faceIndex).endIndex;
			auto startElementInCombinedVector = m_combinedVertexDataMatVector.m_materialIDIndexMap.at(startFaceVertIndex);
			auto endElementInCombinedVector = m_combinedVertexDataMatVector.m_materialIDIndexMap.at(endFaceVertIndex);


			if(startElementInCombinedVector.material != material)
			{
				//vkladany material je rozdielny material od materialu
				//na combinedVector mieste

				const auto& materialIt = m_materialIDVertexDataMap.find(material);

				if(materialIt != m_materialIDVertexDataMap.end())
				{
					//avsak ide o material ktory sa uz nachadza v combinedVectore


					//VectorData newData;
					//miesto vytvarania novej VectorData si musim ziskat existujuci VectorData
					//pre vkladany material
					VectorData& existingVectorData = materialIt->second;

					for (size_t i = startFaceVertIndex; i <= endFaceVertIndex; i++)
					{
						//taktiez musime oznacit elementy ako deleted v materialVectore
						const auto& materialIDVectorIt = m_materialIDVertexDataMap.find(m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).material);
						if (materialIDVectorIt != m_materialIDVertexDataMap.end()) {
							// Element found, update VectorData
							// taktiez musime oznacit v druhom vektore
							materialIDVectorIt->second.m_materialIDIndexMap.at(m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).index).deleted = true;
						}


						existingVectorData.m_materialIDIndexMap.push_back({material, i, false});
						//push_backnut musime aj vertexData k tomu
						existingVectorData.m_vertexData.push_back({m_combinedVertexDataMatVector.m_vertexData.at(i).position, m_combinedVertexDataMatVector.m_vertexData.at(i).normal});

						//a teraz este zamenit tieto elementy na selectedFace indexoch za
						//novy material a k tymto miestam taktiez priradit index

						m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).material = material;
						m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).index = (existingVectorData.m_materialIDIndexMap.size() - 1);
					}

					//no lenze este musime vyriesit to ze sa tam nachadzal na tom mieste material
					//a teraz uz je prepisany
					//preto musime najst ten povodny material a vytvorit pren
					//startEndMovedElement vector

					const auto& startEndMovedElementIt = startEndMovedElementsMap.find(startElementInCombinedVector.material);

					// Check if the element exists
					if (startEndMovedElementIt != startEndMovedElementsMap.end()) {
						// Element found, push the element to the existing vector
						startEndMovedElementIt.operator*().second.startEndOfElements.push_back({ startElementInCombinedVector.index, endElementInCombinedVector.index });
					}
					else {
						// Element not found, create a new vector and insert it into the map
						StartEndMovedElements elements;
						elements.startEndOfElements.push_back({ startElementInCombinedVector.index, endElementInCombinedVector.index });
						startEndMovedElementsMap[startElementInCombinedVector.material] = elements;
					}


				} else
				{
					//v tomto pripade potrebujeme mat movedElements daneho
					//materialVectora

					//avsak ide o material ktory sa nenachadza v combinedVectore
					VectorData newData;

					for (size_t i = startFaceVertIndex; i <= endFaceVertIndex; i++)
					{
						//taktiez musime oznacit elementy ako deleted v materialVectore
						const auto& materialIDVectorIt = m_materialIDVertexDataMap.find(m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).material);
						if (materialIDVectorIt != m_materialIDVertexDataMap.end()) {
							// Element found, update VectorData
							// taktiez musime oznacit v druhom vektore
							materialIDVectorIt->second.m_materialIDIndexMap.at(m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).index).deleted = true;
						}

						newData.m_materialIDIndexMap.push_back({material,i,false});
						//taktiez push_backni aj vertexData
						newData.m_vertexData.push_back({ m_combinedVertexDataMatVector.m_vertexData.at(i).position, m_combinedVertexDataMatVector.m_vertexData.at(i).normal});

						//a teraz este zamenit tieto elementy na selectedFace indexoch za
						//novy material a k tymto miestam taktiez priradit index

						m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).material = material;
						m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).index = (newData.m_materialIDIndexMap.size() - 1);
					}
					m_materialIDVertexDataMap[material] = newData;

					//no lenze este musime vyriesit to ze sa tam nachadzal na tom mieste material
					//a teraz uz je prepisany
					//preto musime najst ten povodny material a vytvorit pren
					//startEndMovedElement vector

					const auto& startEndMovedElementIt = startEndMovedElementsMap.find(startElementInCombinedVector.material);

					// Check if the element exists
					if (startEndMovedElementIt != startEndMovedElementsMap.end()) {
						// Element found, push the element to the existing vector
						startEndMovedElementIt.operator*().second.startEndOfElements.push_back({ startElementInCombinedVector.index, endElementInCombinedVector.index });
					}
					else {
						// Element not found, create a new vector and insert it into the map
						StartEndMovedElements elements;
						elements.startEndOfElements.push_back({ startElementInCombinedVector.index, endElementInCombinedVector.index });
						startEndMovedElementsMap[startElementInCombinedVector.material] = elements;
					}

					//no ako vysledok tohto sme uspesne zmenili material pre danu facu, taktiez
					//sme aktualizovali jej novyIndex v combinedVektore a aj v materialVektore

					//taktiez mame k dispozicii startEndMovedElementsMap pre povodny material, ktory
					//bol prepisany, takze v kludne mozeme vypocitat movedElements neskor
				}

				
			} else
			{
				//vkladany material je rovnaky ako material od materialu
				//na combinedVector mieste

			}
		}

		for (auto& materialStartEnd : startEndMovedElementsMap) {
			std::sort(materialStartEnd.second.startEndOfElements.begin(), materialStartEnd.second.startEndOfElements.end(),
				[](const StartEndOfElement& a, const StartEndOfElement& b) {
					return a.start < b.start;
				});
		}

		for (auto& startEndMovedElement : startEndMovedElementsMap)
		{
			assignMovedElementsOfDeletedFaces(m_materialIDVertexDataMap[startEndMovedElement.first], startEndMovedElement.second.startEndOfElements, startEndMovedElement.second.movedElements);
		}

		//opacne
		for (const auto& startEndMovedElementIt : startEndMovedElementsMap)
		{
			//prechadzaj cez vsetky movedElementy
			for (const auto& movedElement : startEndMovedElementIt.second.movedElements)
			{
				m_combinedVertexDataMatVector.m_materialIDIndexMap.at(movedElement.index).index = movedElement.newIndexInMaterialVector;
			}
		}

		for (auto& materialVectorData : m_materialIDVertexDataMap) {
			auto& materialIDIndexMap = materialVectorData.second.m_materialIDIndexMap;
			auto& vertexData = materialVectorData.second.m_vertexData;

			auto it = materialIDIndexMap.begin();
			while (it != materialIDIndexMap.end()) {
				if (it->deleted) {
					size_t index = std::distance(materialIDIndexMap.begin(), it);

					vertexData.erase(vertexData.begin() + index);

					it = materialIDIndexMap.erase(it);
				}
				else {
					++it;
				}
			}
		}

	}

	void deleteMaterialsOfSelectedFaces(std::vector<HalfEdgeDS::Face*>& selectedFaces)
	{

		std::map<Material*, StartEndMovedElements> startEndMovedElementsMap;

		std::pair<std::vector<StartEndOfElement>, std::vector<MovedElement>> startEndAndMovedElementsCombined;

		for (const auto& selectedFace : selectedFaces)
		{
			size_t faceIndex = std::distance(selectedFace->getMesh().faceIterBegin(), selectedFace->getHalfEdge()->getFace());
			size_t startFaceVertIndex = m_facesData.at(faceIndex).startIndex;
			size_t endFaceVertIndex = m_facesData.at(faceIndex).endIndex;

			startEndAndMovedElementsCombined.first.push_back({ startFaceVertIndex, endFaceVertIndex });

			//teraz musim ziskat start a end indexy pre dany material
			const auto& startElementInCombinedVector = m_combinedVertexDataMatVector.m_materialIDIndexMap.at(startFaceVertIndex);
			const auto& endElementInCombinedVector = m_combinedVertexDataMatVector.m_materialIDIndexMap.at(endFaceVertIndex);


			//staci najst bud len startElement alebo endElement, kedze obidva prisluchaju vzdy k rovnakemu materialu


			const auto& startEndMovedElementIt = startEndMovedElementsMap.find(startElementInCombinedVector.material);

			// Check if the element exists
			if (startEndMovedElementIt != startEndMovedElementsMap.end()) {
				// Element found, push the element to the existing vector
				startEndMovedElementIt.operator*().second.startEndOfElements.push_back({ startElementInCombinedVector.index, endElementInCombinedVector.index });
			}
			else {
				// Element not found, create a new vector and insert it into the map
				StartEndMovedElements elements;
				elements.startEndOfElements.push_back({ startElementInCombinedVector.index, endElementInCombinedVector.index });
				startEndMovedElementsMap[startElementInCombinedVector.material] = elements;
			}


			for (size_t i = startFaceVertIndex; i <= endFaceVertIndex; i++)
			{
				//taktiez musime oznacit v hlavnom vektore
				m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).deleted = true;

				const auto& materialIDVectorIt = m_materialIDVertexDataMap.find(m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).material);
				if (materialIDVectorIt != m_materialIDVertexDataMap.end()) {
					// Element found, update VectorData
					// taktiez musime oznacit v druhom vektore
					materialIDVectorIt->second.m_materialIDIndexMap.at(m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).index).deleted = true;
				}
			}
		}

		//musime sortnut vsetky startAndEndOfDeletedFaces podla start indexu


		std::sort(startEndAndMovedElementsCombined.first.begin(), startEndAndMovedElementsCombined.first.end(),
			[](const StartEndOfElement& a, const StartEndOfElement& b) {
				return a.start < b.start;
			});

		for (auto& materialStartEnd : startEndMovedElementsMap) {
			std::sort(materialStartEnd.second.startEndOfElements.begin(), materialStartEnd.second.startEndOfElements.end(),
				[](const StartEndOfElement& a, const StartEndOfElement& b) {
					return a.start < b.start;
				});
		}

		//toto musim zmenit mal by som mat po kope tieto vektory

		//uz mame taktiez movedElements vsetkych vektorov
		assignMovedElementsOfDeletedFaces(m_combinedVertexDataMatVector,startEndAndMovedElementsCombined.first, startEndAndMovedElementsCombined.second);
		for (auto& startEndMovedElement : startEndMovedElementsMap)
		{
			assignMovedElementsOfDeletedFaces(m_materialIDVertexDataMap[startEndMovedElement.first], startEndMovedElement.second.startEndOfElements, startEndMovedElement.second.movedElements);
		}

		//teraz je mojou ulohou najst movedElementy obidvoch vektorov

		//cize budem prechadzat movedElementy combinedVektora

		for (const auto& startEndAndMovedElementCombined : startEndAndMovedElementsCombined.second)
		{
			MovedElement combinedMovedElement = startEndAndMovedElementCombined;
			MovedElement movedElementInOtherVector;

			Element& elementInOtherVector = m_materialIDVertexDataMap.find(combinedMovedElement.material)->second.m_materialIDIndexMap.at(combinedMovedElement.index);
			elementInOtherVector.index = combinedMovedElement.newIndexInMaterialVector;

		}

		//opacne
		for (const auto& startEndMovedElementIt : startEndMovedElementsMap)
		{
			//prechadzaj cez vsetky movedElementy
			for (const auto& movedElement : startEndMovedElementIt.second.movedElements)
			{
				m_combinedVertexDataMatVector.m_materialIDIndexMap.at(movedElement.index).index = movedElement.newIndexInMaterialVector;
			}
		}

		//Remove all marked elements
		//remove combinedVector
		m_combinedVertexDataMatVector.m_materialIDIndexMap.erase(std::remove_if(m_combinedVertexDataMatVector.m_materialIDIndexMap.begin(), m_combinedVertexDataMatVector.m_materialIDIndexMap.end(),
			[](const Element& element) { return element.deleted; }),
			m_combinedVertexDataMatVector.m_materialIDIndexMap.end());


		//mal by som si este zapisat nejaky vector, ktory by mi hovoril len o materialVektoroch,
		//v ktorych existuje deleted element, momentalne to je tak ze sa prechadza cez uplne vsetky mapy
		//cize sa moze stat ze v nejakej budeme zbytocne kedze sa tam nenachadzaju deleted elements

		//remove all other vectors
		for (auto& materialVectorData : m_materialIDVertexDataMap)
		{
			materialVectorData.second.m_materialIDIndexMap.erase(std::remove_if(materialVectorData.second.m_materialIDIndexMap.begin(), materialVectorData.second.m_materialIDIndexMap.end(),
				[](const Element& element) { return element.deleted; }),
				materialVectorData.second.m_materialIDIndexMap.end());
		}

	}

	HalfEdgeDS::HalfEdgeMesh* getMeshData()
	{
		return m_halfEdgeMesh;
	}

private:

	/*
	void setRenderingData()
	{
		//edges
		for (auto& edge : m_halfEdgeMesh->m_edges)
		{
			glm::vec3 startPoint = (*edge.getFirstVertex()).getPosition();
			startPoint.y += 0.005f;
			glm::vec3 endPoint = (*edge.getSecondVertex()).getPosition();
			endPoint.y += 0.005f;
			m_meshRenderingData.m_edges.push_back({ startPoint, false });
			m_meshRenderingData.m_edges.push_back({ endPoint, false });
		}

		//vertices
		for (auto& vertex : m_halfEdgeMesh->m_vertices)
		{
			m_meshRenderingData.m_points.push_back({ vertex.getPosition(), false});
		}
	}
	*/

	void assignMovedElementsOfDeletedFaces(const VectorData& currentVectorData,const std::vector<StartEndOfElement>& startAndEndOfDeletedFaces, std::vector<MovedElement>& movedElements)
	{
		size_t moveElementByAmount = 0;
		//teraz si zistim vsetky movedElementy a pridam ich do movedElements
		for (int j = 0; j < startAndEndOfDeletedFaces.size(); ++j)
		{
			int startIndex;
			int endIndex;
			//prveho end
			startIndex = startAndEndOfDeletedFaces.at(j).end + 1;

			if (startIndex != currentVectorData.m_materialIDIndexMap.size())
			{

				//druheho start
				if ((j + 1) < startAndEndOfDeletedFaces.size())
				{
					endIndex = startAndEndOfDeletedFaces.at(j + 1).start - 1;
				}
				else
				{
					//ak nie je uz startAndEndOfDeletedFaces tak musime sa dostat az po konec combinedVektora
					endIndex = currentVectorData.m_materialIDIndexMap.size() - 1;
				}

				//musime skontrolovat ci druha faca nenasleduje okamzite za prvou
				//v takomto pripade nemame ako vytvorit movedElement

				moveElementByAmount += (startAndEndOfDeletedFaces.at(j).end - startAndEndOfDeletedFaces.at(j).start) + 1;

				//pridat MovedElements do vektora
				for (size_t k = startIndex; k <= endIndex; ++k)
				{
					movedElements.push_back({currentVectorData.m_materialIDIndexMap.at(k).material, currentVectorData.m_materialIDIndexMap.at(k).index, k - moveElementByAmount});
				}

			}
		}
	}
};
