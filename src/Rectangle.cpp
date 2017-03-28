#include <Geometry.h>

#include <World.h>

namespace RE
{
	Rectangle::Rectangle()
	{

	}

	bool Rectangle::Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const
	{
		VML::Vector normal(0.0f, 0.0f, 1.0f, 0.0f);
		F32 dDOTn = ray.GetDirection().v3Dot(normal);
		if (VML::FEquals(dDOTn, 0.0f))
			return false;

		VML::Vector p0(-1.0f, -1.0f, 0.0f, 1.0f);
		F32 t = (p0 - ray.GetOrigin()).v3Dot(normal) / dDOTn;

		if (t <= VML::FLOAT_EPSILON)
			return false;

		VML::Vector p = ray.GetPointAlongRay(t);
		VML::Vector d = p - p0;

		VML::Vector a = VML::Vector(1.0f, 0.0f, 0.0f, 0.0f);
		VML::Vector b = VML::Vector(0.0f, 1.0f, 0.0f, 0.0f);

		F32 dDOTa = d.v3Dot(a);
		if (dDOTa < 0.0f || dDOTa > a.v3LengthSq())
			return false;

		F32 dDOTb = d.v3Dot(b);
		if (dDOTb < 0.0f || dDOTb > b.v3LengthSq())
			return false;

		outHitInfo.closestIntersection.t = t;
		outHitInfo.closestIntersection.normal = normal;
		outHitInfo.closestIntersection.worldCoords = p;
		
		outHitInfo.intersections.push_back(outHitInfo.closestIntersection);
		outHitInfo.numIntersections++;

		return true;
	}

	bool Rectangle::Intersects(F32& tmin, const Ray& ray)const
	{
		VML::Vector normal(0.0f, 0.0f, 1.0f, 0.0f);
		F32 dDOTn = ray.GetDirection().v3Dot(normal);
		if (VML::FEquals(dDOTn, 0.0f))
			return false;

		VML::Vector p0(-1.0f, -1.0f, 0.0f, 1.0f);
		tmin = (p0 - ray.GetOrigin()).v3Dot(normal) / dDOTn;

		if (tmin <= VML::FLOAT_EPSILON)
			return false;

		VML::Vector p = ray.GetPointAlongRay(tmin);
		VML::Vector d = p - p0;

		VML::Vector a = VML::Vector(1.0f, 0.0f, 0.0f, 0.0f);
		VML::Vector b = VML::Vector(0.0f, 1.0f, 0.0f, 0.0f);

		F32 dDOTa = d.v3Dot(a);
		if (dDOTa < 0.0f || dDOTa > a.v3LengthSq())
			return false;

		F32 dDOTb = d.v3Dot(b);
		if (dDOTb < 0.0f || dDOTb > b.v3LengthSq())
			return false;

		return true;
	}

	BoundingBox Rectangle::GetBoundingBox()const
	{
		VML::VECTOR3F min(-1.0f, -1.0f, -VML::FLOAT_EPSILON);
		VML::VECTOR3F max(1.0f, 1.0f, VML::FLOAT_EPSILON);

		return BoundingBox(min, max);
	}




	F32 Rectangle::PDF(const ElementIntersection& ei)const
	{
		return invArea;
	}

	VML::Vector Rectangle::Sample()
	{
		VML::VECTOR2F s = sampler.SampleUnitSquare();
		return VML::VectorZero();
	}
}