#include "Geometry.h"

namespace RE
{
	Sphere::Sphere()
	{
		this->origin = VML::Vector(0.0f, 0.0f, 0.0f, 1.0f);
		this->radius = 1.0f;

		boundingSphere = BoundingSphere(origin, radius);
	}

	Sphere::Sphere(const VML::VECTOR3F& origin, F32 radius)
	{
		this->origin = VML::Vector(origin.x, origin.y, origin.z, 1.0f);
		this->radius = radius;

		boundingSphere = BoundingSphere(this->origin, radius);
	}

	Sphere::Sphere(const VML::Vector& origin, F32 radius)
	{
		this->origin = VML::Vector(origin);
		this->radius = radius;

		boundingSphere = BoundingSphere(origin, radius);
	}

	bool Sphere::Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const
	{
		return boundingSphere.Intersects(outHitInfo, ray);
	}
}