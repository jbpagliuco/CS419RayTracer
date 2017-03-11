#pragma once

#include <VML.h>
#include <Util.h>
#include <Transform.h>

namespace RE
{
	// Forward declaration
	struct RayIntersectionList;
	class Ray;

	class BoundingVolume
	{
	public:
		// Default constructor.
		BoundingVolume() = default;

		// Default destructor.
		virtual ~BoundingVolume() = default;

		// Checks to see if a ray intersects this bounding volume.
		// @param [out] outHitInfo - A list of the intersection points.
		// @param ray - The ray to check against.
		// @return Does the ray intersect this bounding volume?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const = 0;
	};

	RE_ALIGN_MS(16) class BoundingSphere : public BoundingVolume
	{
	public:
		// Creates a unit bounding sphere.
		BoundingSphere();
		// Creates a bounding sphere with the specified origin and radius.
		BoundingSphere(const VML::Vector& origin, F32 radius);

		// Default destructor.
		virtual ~BoundingSphere() = default;

		// Checks to see if a ray intersects this bounding volume.
		// @param [out] outHitInfo - A list of the intersection points.
		// @param ray - The ray to check against.
		// @return Does the ray intersect this bounding volume?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const override;

	private:
		VML::Vector origin;
		F32 radius;
	} RE_ALIGN_GCC(16);

	class BoundingBox : BoundingVolume
	{
	public:
		BoundingBox();
		BoundingBox(const VML::VECTOR3F& min, const VML::VECTOR3F& max);

		virtual ~BoundingBox();

		virtual bool operator==(const BoundingBox& other)const;

		// Checks to see if a ray intersects this bounding volume.
		// @param [out] outHitInfo - A list of the intersection points.
		// @param ray - The ray to check against.
		// @return Does the ray intersect this bounding volume?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const override;

		// Checks to see if a ray intersects this bounding volume.
		// @param [out] outHitInfo - A list of the intersection points.
		// @param ray - The ray to check against.
		// @return Does the ray intersect this bounding volume?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray, D64& t0, Vector3<D64>& tMin, Vector3<D64>& tMax)const;

		// Is this point inside of this box?
		// @param point - The point to check.
		virtual bool IsInside(const VML::Vector& point)const;

		// Returns the center coordinate of this bounding volume.
		VML::VECTOR3F GetCenter()const;

		// Returns the longest axis of this bounding volume.
		U8 LongestAxis()const;

		// Extends this bounding box.
		// @param other - The bounding box to extend to.
		void Extend(const BoundingBox& other);

		// Returns the transformed version of this bounding box.
		BoundingBox TransformBox(const Transform& transform)const;

	public:
		VML::VECTOR3F min, max;
	};
}