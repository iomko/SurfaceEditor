#pragma once
#include "../TrianglePP/tpp_interface.hpp"
#include <vector>
#include <iostream>
#include <cassert>


using namespace tpp;

struct DelaunayVertex
{
	double x = 0.0;
	double y = 0.0;
};

struct DelaunayTriangle
{
	DelaunayVertex vertex1;
	DelaunayVertex vertex2;
	DelaunayVertex vertex3;
};


class DelaunayTest
{
public:
	static void classicTest()
	{
		//DATA
		std::vector<Delaunay::Point> delaunayInput;

		delaunayInput.push_back(Delaunay::Point(5.38461, 2.49756)); //0 - index 0
		delaunayInput.push_back(Delaunay::Point(10.7969, -6.18571)); //1 - index 1
		delaunayInput.push_back(Delaunay::Point(3.59558, -3.68423)); //2 - index 2
		delaunayInput.push_back(Delaunay::Point(-2.01404, -8.78555)); //4 - index 3
		delaunayInput.push_back(Delaunay::Point(-6.17731, -5.06843)); //5 - index 4
		delaunayInput.push_back(Delaunay::Point(1.60165, -1.69653)); //3 - index 5

		// 1. standard triangulation
		Delaunay trGenerator(delaunayInput);
		trGenerator.Triangulate();

		//Store final output
		std::vector<DelaunayTriangle> delaunayOut;

		// iterate over triangles
		for (FaceIterator fit = trGenerator.fbegin(); fit != trGenerator.fend(); ++fit)
		{
			int vertexIdx1 = fit.Org();
			int vertexIdx2 = fit.Dest();
			int vertexIdx3 = fit.Apex();

			// access data
			double x1 = delaunayInput[vertexIdx1][0];
			double y1 = delaunayInput[vertexIdx1][1];

			double x2 = delaunayInput[vertexIdx2][0];
			double y2 = delaunayInput[vertexIdx2][1];

			double x3 = delaunayInput[vertexIdx3][0];
			double y3 = delaunayInput[vertexIdx3][1];

			DelaunayVertex dVertex1;
			dVertex1.x = x1;
			dVertex1.y = y1;

			DelaunayVertex dVertex2;
			dVertex2.x = x2;
			dVertex2.y = y2;

			DelaunayVertex dVertex3;
			dVertex3.x = x3;
			dVertex3.y = y3;

			DelaunayTriangle dTriangle;
			dTriangle.vertex1 = dVertex1;
			dTriangle.vertex2 = dVertex2;
			dTriangle.vertex3 = dVertex3;

			delaunayOut.push_back(dTriangle);
		}

		for (DelaunayTriangle& dTriangle : delaunayOut)
		{
			std::cout << "---Triangle: " << std::endl;
			std::cout << "------Vertex1: " << "x: " << dTriangle.vertex1.x << ", y: " << dTriangle.vertex1.y << std::endl;
			std::cout << "------Vertex2: " << "x: " << dTriangle.vertex2.x << ", y: " << dTriangle.vertex2.y << std::endl;
			std::cout << "------Vertex3: " << "x: " << dTriangle.vertex3.x << ", y: " << dTriangle.vertex3.y << std::endl;
		}
	}
	
	static void constrainedTest()
	{
		//skusme najskor standard triangulation
		//ci aspon toto funguje
		//avsak s mojimi datami

		//DATA
		std::vector<Delaunay::Point> delaunayInput;

		delaunayInput.push_back(Delaunay::Point(5.38461, 2.49756)); //0 - index 0
		delaunayInput.push_back(Delaunay::Point(10.7969, -6.18571)); //1 - index 1
		delaunayInput.push_back(Delaunay::Point(3.59558, -3.68423)); //2 - index 2
		delaunayInput.push_back(Delaunay::Point(-2.01404, -8.78555)); //4 - index 3
		delaunayInput.push_back(Delaunay::Point(-6.17731, -5.06843)); //5 - index 4
		delaunayInput.push_back(Delaunay::Point(1.60165, -1.69653)); //3 - index 5
		delaunayInput.push_back(Delaunay::Point(5.38461, 2.49756)); //0 - index 6

		// 1. standard triangulation
		Delaunay trGenerator(delaunayInput);

		//Store final output
		std::vector<DelaunayTriangle> delaunayOut;

		// 2. triangulation with quality constraints
		bool enforceQuality = true;

		// 4. constrained Delaunay
		std::vector<Delaunay::Point> segments;
		for (size_t i = 0; i + 1 < delaunayInput.size(); ++i)
			segments.push_back(delaunayInput[i]), segments.push_back(delaunayInput[i + 1]);

		trGenerator.setSegmentConstraint(segments);
		trGenerator.Triangulate();  // Don't pass 'true' if you don't want steiner points
		int triCount = trGenerator.triangleCount(); // how many?

		// iterate over triangles
		for (FaceIterator fit = trGenerator.fbegin(); fit != trGenerator.fend(); ++fit)
		{
			int vertexIdx1 = fit.Org();
			int vertexIdx2 = fit.Dest();
			int vertexIdx3 = fit.Apex();

			// access data
			double x1 = delaunayInput[vertexIdx1][0];
			double y1 = delaunayInput[vertexIdx1][1];

			double x2 = delaunayInput[vertexIdx2][0];
			double y2 = delaunayInput[vertexIdx2][1];

			double x3 = delaunayInput[vertexIdx3][0];
			double y3 = delaunayInput[vertexIdx3][1];

			DelaunayVertex dVertex1;
			dVertex1.x = x1;
			dVertex1.y = y1;

			DelaunayVertex dVertex2;
			dVertex2.x = x2;
			dVertex2.y = y2;

			DelaunayVertex dVertex3;
			dVertex3.x = x3;
			dVertex3.y = y3;

			DelaunayTriangle dTriangle;
			dTriangle.vertex1 = dVertex1;
			dTriangle.vertex2 = dVertex2;
			dTriangle.vertex3 = dVertex3;

			delaunayOut.push_back(dTriangle);
		}

		for (DelaunayTriangle& dTriangle : delaunayOut)
		{
			std::cout << "---Triangle: " << std::endl;
			std::cout << "------Vertex1: " << "x: " << dTriangle.vertex1.x << ", y: " << dTriangle.vertex1.y << std::endl;
			std::cout << "------Vertex2: " << "x: " << dTriangle.vertex2.x << ", y: " << dTriangle.vertex2.y << std::endl;
			std::cout << "------Vertex3: " << "x: " << dTriangle.vertex3.x << ", y: " << dTriangle.vertex3.y << std::endl;
		}
		
	}
};