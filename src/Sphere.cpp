#include "Geometry.h"

namespace RE
{
	Sphere::Sphere() : radius(1.0f)
	{
	}

	Sphere::Sphere(F32 radius) : radius(radius)
	{
	}

	bool Sphere::Intersects(RayIntersectionList& outHitInfo, const Ray& ray, const Transform& elementTransform)const
	{
		return BoundingSphere(elementTransform.position, radius).Intersects(outHitInfo, ray);
	}


	bool Sphere::Intersects(F32& tmin, const Ray& ray, const Transform& elementTransform)const
	{
		VML::Vector o = ray.GetOrigin();
		VML::Vector d = ray.GetDirection();
		VML::Vector dist = o - elementTransform.position;

		F32 a = d.v3Dot(d);
		F32 b = ((o - elementTransform.position) * 2).v3Dot(d);
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

			bool bHit = false;

			F32 t = (-b + e) / den;
			if (t > VML::FLOAT_EPSILON)
			{
				tmin = t;
				bHit = true;

				if (VML::FEquals(e, 0.0f))
					return true;
			}

			t = (-b - e) / den;
			if (t > VML::FLOAT_EPSILON)
			{
				tmin = t;
				bHit = true;
			}

			if (bHit)
				return true;
		}

		return false;
	}



	BoundingBox Sphere::GetBoundingBox()const
	{
		VML::VECTOR3F min(-radius, -radius, -radius);
		VML::VECTOR3F max(radius, radius, radius);
		return BoundingBox(min, max);
	}
}