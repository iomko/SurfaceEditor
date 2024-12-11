#pragma once

//least squares CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/linear_least_squares_fitting_3.h>
#include <iostream>
#include <vector>

//
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_3 Point_3;
typedef K::Plane_3 Plane_3;
typedef std::vector<Point_3> PointList;
//

#include <Eigen/Core>
#include <Eigen/Eigenvalues>

class VectorOperations
{
public:
	static std::tuple<Eigen::Vector3d, Eigen::Vector3d, double> calcLeastSquaresPlane(const std::vector<Eigen::Vector3d>& points) {
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

	static bool areVectorsCoplanar(const PointList& points, double threshold = 1e-6) {
		Plane_3 plane;
		linear_least_squares_fitting_3(points.begin(), points.end(),
			plane, CGAL::Dimension_tag<0>());

		auto normal = plane.orthogonal_vector();

		std::vector<double> residuals;
		residuals.reserve(points.size());
		for (const auto& point : points) {
			double distance = (point - plane.point()) * normal;
			residuals.push_back(std::abs(distance));
		}

		double meanSquaredError = 0.0;
		for (const auto& residual : residuals) {
			meanSquaredError += residual * residual;
		}
		meanSquaredError /= residuals.size();

		return meanSquaredError <= threshold;
	}

	static float angleBetweenVectors(const glm::vec3& a, const glm::vec3& b) {
		float dotProduct = glm::dot(a, b);

		// Calculate the magnitudes of the vectors
		float magnitudeA = glm::length(a);
		float magnitudeB = glm::length(b);

		float angleRadians = std::acos(dotProduct / (magnitudeA * magnitudeB));

		float angleDegrees = glm::degrees(angleRadians);
		return angleDegrees;
	}

	static glm::vec3 reflectVector(const glm::vec3& vector, const glm::vec3& normal)
	{
		glm::vec3 reflectedVector = vector - 2.0f * glm::dot(vector, normal) * normal;
		return reflectedVector;
	}

};