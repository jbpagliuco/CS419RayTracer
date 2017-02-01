#include "Geometry.h"

namespace RE
{
	Plane::Plane()
	{
		this->point = VML::VectorZero();
		this->normal = VML::Vector(0.0f, 0.0f, 1.0f, 0.0f);
	}

	Plane::Plane(const VML::VECTOR3F& point, const VML::VECTOR3F& normal)
	{
		this->point = VML::Vector(point);
		this->normal = VML::Vector(normal.x, normal.y, normal.z, 0.0f).v3Normalize();
	}

	Plane::Plane(const VML::Vector& point, const VML::Vector& normal)
	{
		this->point = VML::Vector(point);
		this->normal = VML::Vector(normal).v3Normalize();
	}

	bool Plane::Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const
	{
		F32 dn = ray.GetDirection().v3Dot(normal);
		F32 num = (point - ray.GetOrigin()).v3Dot(normal);

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
}