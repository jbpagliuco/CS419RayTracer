#include "Geometry.h"

namespace RE
{
	Plane::Plane() : normal(0.0f, 1.0f, 0.0f, 0.0f)
	{
	}

	Plane::Plane(const VML::VECTOR3F& normal) : normal(normal)
	{
		this->normal.v3Normalize();
	}

	Plane::Plane(const VML::Vector& normal) : normal(normal)
	{
		this->normal.v3Normalize();
	}

	bool Plane::Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const
	{
		F32 dn = ray.GetDirection().v3Dot(normal);
		F32 num = ray.GetOrigin().negate().v3Dot(normal);

		if (VML::FEquals(dn, 0.0f))
			return false;

		F32 t = num / dn;
		if (t >= VML::FLOAT_EPSILON)
		{
			outHitInfo.closestIntersection.t = t;
			outHitInfo.closestIntersection.worldCoords = ray.GetPointAlongRay(t);
			outHitInfo.closestIntersection.normal = normal;

			outHitInfo.intersections.push_back(outHitInfo.closestIntersection);
			outHitInfo.numIntersections++;

			return true;
		}

		return false;
	}

	bool Plane::Intersects(F32& tmin, const Ray& ray)const
	{
		F32 dn = ray.GetDirection().v3Dot(normal);
		F32 num = ray.GetOrigin().negate().v3Dot(normal);

		if (VML::FEquals(dn, 0.0f))
			return false;

		F32 t = num / dn;
		if (t >= VML::FLOAT_EPSILON)
		{
			tmin = t;
			return true;
		}

		return false;
	}

	BoundingBox Plane::GetBoundingBox()const
	{
		return BoundingBox();
	}

	bool Plane::HasBounds()const
	{
		return false;
	}
}