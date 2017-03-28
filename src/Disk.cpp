#include <Geometry.h>

namespace RE
{
	Disk::Disk(F32 radius, VML::VECTOR3F normal) :
		normal(normal), radiusSq(radius*radius)
	{
		this->normal.v3Normalize();
	}

	bool Disk::Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const
	{
		F32 dn = ray.GetDirection().v3Dot(normal);
		if (VML::FEquals(dn, 0.0f))
			return false;

		F32 t = ray.GetOrigin().negate().v3Dot(normal) / dn;
		if (t <= VML::FLOAT_EPSILON)
			return false;

		VML::Vector p = ray.GetPointAlongRay(t);

		F32 distSqr = p.v3LengthSq();
		if (distSqr < radiusSq)
		{
			RayIntersection r;
			r.t = t;
			r.normal = normal;
			r.worldCoords = p;

			outHitInfo.intersections.push_back(r);
			outHitInfo.closestIntersection = r;
			outHitInfo.numIntersections++;

			return true;
		}

		return false;
	}

	bool Disk::Intersects(F32& tmin, const Ray& ray)const
	{
		F32 dn = ray.GetDirection().v3Dot(normal);
		if (VML::FEquals(dn, 0.0f))
			return false;

		F32 t = ray.GetOrigin().negate().v3Dot(normal) / dn;
		if (t <= VML::FLOAT_EPSILON)
			return false;

		VML::Vector p = ray.GetPointAlongRay(t);

		F32 distSqr = p.v3LengthSq();
		if (distSqr < radiusSq)
		{
			tmin = t;
			return true;
		}

		return false;
	}

	BoundingBox Disk::GetBoundingBox()const
	{
		F32 r = sqrt(radiusSq);
		VML::VECTOR3F min(-r, -r, -r);
		VML::VECTOR3F max(r, r, r);
		return BoundingBox(min, max);
	}
}