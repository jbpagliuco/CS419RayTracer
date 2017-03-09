#pragma once

#include <Util.h>
#include <VML.h>

namespace RE
{
	// Epsilon value to change shadow ray origins.
	const F32 RAY_EPSILON = (10 * VML::FLOAT_EPSILON);

	class Ray
	{
	public:
		// Creates a ray starting at the origin and moving in the (0, 0, 1) direction.
		Ray();

		// Creates a ray.
		// @param origin - The starting point of this ray.
		// @param direction - The direction of this ray.
		// @param epsilon - Move the origin of the ray a small amount in the direction of the ray.
		Ray(const VML::VECTOR3F& origin, const VML::VECTOR3F& direction);

		// Creates a ray.
		// @param origin - The starting point of this ray.
		// @param direction - The direction of this ray.
		Ray(const VML::Vector& origin, const VML::Vector& direction, F32 epsilon = 0.0f);

		// Default destructor.
		virtual ~Ray() = default;

		// Gets a point along the ray.
		// @param t - The distance from the ray's origin.
		VML::Vector GetPointAlongRay(F32 t)const;

		// Gets a copy of the origin point of this ray.
		VML::Vector GetOrigin()const;
		// Gets a copy of a direction vector of this ray.
		VML::Vector GetDirection()const;
		// Returns the inverse of the direction vector (1 / direction)
		VML::VECTOR3F GetInvDirection()const;

	private:
		VML::Vector origin;
		VML::Vector direction;
		VML::VECTOR3F invD;
	};

	struct RayIntersection
	{
		// Distance along the ray.
		F32 t;

		// World coordinates of intersection.
		VML::Vector worldCoords;

		// Normal vector at this intersection.
		VML::Vector normal;

		VML::VECTOR2F uv;
	};

	struct RayIntersectionList
	{
		RayIntersectionList()
		{
			intersections = std::vector<RayIntersection>();
			numIntersections = 0;
		}

		// The closest intersection to the ray origin.
		RayIntersection closestIntersection;

		// A list of all intersection points.
		std::vector<RayIntersection> intersections;

		// The number of intersections with this geometry.
		U32 numIntersections;
	};
}