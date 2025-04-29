#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "ConversionUtils.h"
#include "TrianglePP/tpp_interface.hpp"
#include "ProjectionUtils.h"
#include "PolygonOperations.h"

class Triangulator
{
private:
	static tpp::Delaunay createDelaunay(const std::vector<tpp::Delaunay::Point>& vertices)
	{
		tpp::Delaunay delaunayGenerator(vertices);

		std::vector<tpp::Delaunay::Point> segments;
		for (size_t i = 0; i + 1 < vertices.size(); ++i)
		{
			segments.push_back(vertices[i]);
			segments.push_back(vertices[i + 1]);
		}

		delaunayGenerator.setSegmentConstraint(segments);
		delaunayGenerator.Triangulate();

		return delaunayGenerator;
	}

	static glm::vec3 tryProjectOnPlane(const Plane& plane, 
		const glm::vec3& vertex, const glm::vec3& vertexNormal)
	{
		glm::vec3 retVertex{};
		bool hitPlane = false;
		retVertex = PolygonOperations::intersectRayWithPlane(plane, vertex, vertexNormal, hitPlane);
		if(!hitPlane)
		{
			retVertex = PolygonOperations::intersectRayWithPlane(plane, vertex, -vertexNormal, hitPlane);
		}

		return retVertex;
	}

	static glm::vec3 liftTo3D(const ProjectionAxis& projectionAxis, 
		const tpp::Delaunay::Point& vertex)
	{
		glm::vec3 retVertex{};
		if (projectionAxis == ProjectionAxis::ZY)
		{
			retVertex.x = 0.0f;
			retVertex.y = (float)vertex[1];
			retVertex.z = (float)vertex[0];
		} else if (projectionAxis == ProjectionAxis::ZX)
		{
			retVertex.y = 0.0f;
			retVertex.z = (float)vertex[1];
			retVertex.x = (float)vertex[0];
		} else if(projectionAxis == ProjectionAxis::XY)
		{
			retVertex.x = (float)vertex[0];
			retVertex.y = (float)vertex[1];
			retVertex.z = 0.0f;
		}
		return retVertex;
	}

public:
	static std::vector<glm::vec3> triangulatePolygon(const std::vector<glm::vec3>& vertices)
	{
		//---VERTICES_OF_TRIANGULATED_FACE---
		std::vector<glm::vec3> retResult;

		//---CREATE_PLANE_FROM_VERTICES---
		//to check if all points are coplanar, for now we will want that
		bool allPointsCoplanar;
		Plane plane = PolygonOperations::computePlaneFromPoints(vertices, allPointsCoplanar);

		if(allPointsCoplanar)
		{
			//---PROJECT_ON_THE_RIGHT_PLANE---
			std::vector<glm::vec2> projectedVertices;
			glm::vec3 projectionNormal{};
			//---LAMBDA_TO_COMPUTE_PROJECTION_AND_RETURN_THE_AREA_SIZE---
			auto computeProjectedArea = [&vertices, &projectedVertices](const ProjectionAxis& projectionAxis)
				{
					projectedVertices = ProjectionUtils::projectVertices(vertices, projectionAxis);
					return PolygonOperations::computePolygonArea(projectedVertices);
				};

			if(computeProjectedArea(ProjectionAxis::ZY) > 1e-5f)
			{
				//---SET_PROJECTION_NORMAL_FOR_GETTING_NEW_PROJECTED_POINT_AFTER_TRIANGULATION---
				projectionNormal.x = 1.0f;

				//---DELAUNAY---
				std::vector<tpp::Delaunay::Point> delaunayVertices = ConversionUtils::toDelaunayPoints(projectedVertices);
				tpp::Delaunay delaunayGenerator = createDelaunay(delaunayVertices);

				//---ITERATE_OVER_TRIANGLES---
				for (tpp::FaceIterator fit = delaunayGenerator.fbegin(); fit != delaunayGenerator.fend(); ++fit)
				{
					std::vector<glm::vec3> faceVertices = {
						liftTo3D(ProjectionAxis::ZY, delaunayVertices[fit.Org()]),
						liftTo3D(ProjectionAxis::ZY, delaunayVertices[fit.Dest()]),
						liftTo3D(ProjectionAxis::ZY, delaunayVertices[fit.Apex()])
					};

					for (const auto& faceVertex : faceVertices) {
						glm::vec3 projectedVertex = tryProjectOnPlane(plane, faceVertex, projectionNormal);
						retResult.emplace_back(projectedVertex);
					}
				}

			} else if(computeProjectedArea(ProjectionAxis::ZX) > 1e-5f)
			{
				//---SET_PROJECTION_NORMAL_FOR_GETTING_NEW_PROJECTED_POINT_AFTER_TRIANGULATION---
				projectionNormal.y = 1.0f;

				//---DELAUNAY---
				std::vector<tpp::Delaunay::Point> delaunayVertices = ConversionUtils::toDelaunayPoints(projectedVertices);
				tpp::Delaunay delaunayGenerator = createDelaunay(delaunayVertices);

				//---ITERATE_OVER_TRIANGLES---
				for (tpp::FaceIterator fit = delaunayGenerator.fbegin(); fit != delaunayGenerator.fend(); ++fit)
				{
					std::vector<glm::vec3> faceVertices = {
						liftTo3D(ProjectionAxis::ZX, delaunayVertices[fit.Org()]),
						liftTo3D(ProjectionAxis::ZX, delaunayVertices[fit.Dest()]),
						liftTo3D(ProjectionAxis::ZX, delaunayVertices[fit.Apex()])
					};

					for (const auto& faceVertex : faceVertices) {
						glm::vec3 projectedVertex = tryProjectOnPlane(plane, faceVertex, projectionNormal);
						retResult.emplace_back(projectedVertex);
					}
				}

			} else if(computeProjectedArea(ProjectionAxis::XY) > 1e-5f)
			{
				//---SET_PROJECTION_NORMAL_FOR_GETTING_NEW_PROJECTED_POINT_AFTER_TRIANGULATION---
				projectionNormal.z = 1.0f;

				//---DELAUNAY---
				std::vector<tpp::Delaunay::Point> delaunayVertices = ConversionUtils::toDelaunayPoints(projectedVertices);
				tpp::Delaunay delaunayGenerator = createDelaunay(delaunayVertices);

				//---ITERATE_OVER_TRIANGLES---
				for (tpp::FaceIterator fit = delaunayGenerator.fbegin(); fit != delaunayGenerator.fend(); ++fit)
				{
					std::vector<glm::vec3> faceVertices = {
						liftTo3D(ProjectionAxis::XY, delaunayVertices[fit.Org()]),
						liftTo3D(ProjectionAxis::XY, delaunayVertices[fit.Dest()]),
						liftTo3D(ProjectionAxis::XY, delaunayVertices[fit.Apex()])
					};

					for (const auto& faceVertex : faceVertices) {
						glm::vec3 projectedVertex = tryProjectOnPlane(plane, faceVertex, projectionNormal);
						retResult.emplace_back(projectedVertex);
					}
				}
			}
		}
		return retResult;
	}

};
