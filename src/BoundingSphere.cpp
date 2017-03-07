#include <BoundingVolume.h>

#include <Geometry.h>

namespace RE
{
	BoundingSphere::BoundingSphere()
	{
		origin = VML::VectorZero();
		radius = 1.0f;
	}

	BoundingSphere::BoundingSphere(const VML::Vector& origin, F32 radius)
	{
		this->origin = origin;
		this->radius = radius;
	}

	bool BoundingSphere::Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const
	{
		VML::Vector o = ray.GetOrigin();
		VML::Vector d = ray.GetDirection();
		VML::Vector dist = o - origin;

		F32 a = d.v3Dot(d);
		F32 b = ((o - origin) * 2).v3Dot(d);
		F32 c = (dist.v3Dot(dist)) - (radius * radius);

		F32 disc = (b * b) - (4 * a * c);

		if (disc < 0)
		{
			return false;
		}
		else
		{
			F32 e = sqrt(disc);
			F32 den = 2.0f * a;

			F32 t = (-b + e) / den;
			if (t > VML::FLOAT_EPSILON)
			{
				outHitInfo.closestIntersection.t = t;
				outHitInfo.closestIntersection.worldCoords = ray.GetPointAlongRay(t);
				outHitInfo.closestIntersection.normal = (outHitInfo.closestIntersection.worldCoords - origin).v3Normalize();

				outHitInfo.intersections.push_back(outHitInfo.closestIntersection);
				outHitInfo.numIntersections++;

				if (VML::FEquals(e, 0.0f))
					return true;
			}

			t = (-b - e) / den;
			if (t > VML::FLOAT_EPSILON)
			{
				outHitInfo.closestIntersection.t = t;
				outHitInfo.closestIntersection.worldCoords = ray.GetPointAlongRay(t);
				outHitInfo.closestIntersection.normal = (outHitInfo.closestIntersection.worldCoords - origin).v3Normalize();

				outHitInfo.intersections.push_back(outHitInfo.closestIntersection);
				outHitInfo.numIntersections++;
			}

			if (outHitInfo.numIntersections > 0)
				return true;
		}

		return false;
	}
}