#pragma once

#include <VML.h>
#include "Util.h"
#include <Transform.h>
#include <map>
#include <BoundingVolume.h>

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
	


	





	class Geometry
	{
	public:
		virtual ~Geometry() = default;

		// Checks to see if a ray intersects this geometry.
		// @param [out] outHitInfo - A list of the intersection points.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray, const Transform& elementTransform)const = 0;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] t - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& t, const Ray& ray, const Transform& elementTransform)const = 0;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const = 0;
		
		// Can this object be bounding? (ie planes cannot)
		virtual bool HasBounds()const;
	};

	class Plane : public Geometry
	{
	public:
		// Creates a normal of (0, 0, 1).
		Plane();

		// Creates a plane.
		// @param normal - The normal of the plane.
		Plane(const VML::VECTOR3F& normal);

		// Creates a plane.
		// @param normal - The normal of the plane.
		Plane(const VML::Vector& normal);

		// Default destructor.
		virtual ~Plane() = default;

		// Checks to see if a ray intersects this geometry.
		// @param [out] outHitInfo - A list of the intersection points.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray, const Transform& elementTransform)const;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] tmin - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& tmin, const Ray& ray, const Transform& elementTransform)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;

		// Can this object be bounding? (ie planes cannot)
		virtual bool HasBounds()const;

	protected:
		VML::Vector normal;
	};

	class Sphere : public Geometry
	{
	public:
		// Creates a unit sphere.
		Sphere();
		
		// Creates a sphere.
		// @param radius - The radius of the sphere.
		Sphere(F32 radius);

		// Default destructor.
		virtual ~Sphere() = default;

		// Checks to see if a ray interects this sphere.
		// @param outHitInfo - Describes the point of intersection.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this sphere?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray, const Transform& elementTransform)const override;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] tmin - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& tmin, const Ray& ray, const Transform& elementTransform)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;

	protected:
		F32 radius;
	};

	class Triangle : public Geometry
	{
	public:
		Triangle();

		Triangle(VML::VECTOR3F p1, VML::VECTOR3F p2, VML::VECTOR3F p3);

		// Checks to see if a ray interects this sphere.
		// @param outHitInfo - Describes the point of intersection.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this sphere?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray, const Transform& elementTransform)const override;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] tmin - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& tmin, const Ray& ray, const Transform& elementTransform)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;

	private:
		VML::VECTOR3F p1, p2, p3;
		VML::Vector normal;
	};

	RE_ALIGN_MS(16) class Disk : public Geometry
	{
	public:
		Disk(F32 radius, VML::VECTOR3F normal);

		// Checks to see if a ray interects this geometry.
		// @param outHitInfo - Describes the point of intersection.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray, const Transform& elementTransform)const override;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] tmin - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& tmin, const Ray& ray, const Transform& elementTransform)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;

	protected:
		VML::Vector normal;
		F32 radiusSq;
	} RE_ALIGN_GCC(16);





	Geometry * LoadGeometry(const std::map<std::string, std::string>& params);
	Geometry * LoadGeometry(const std::string& params);
}