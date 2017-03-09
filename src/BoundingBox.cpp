#include <BoundingVolume.h>

#include <Geometry.h>
#include <algorithm>

namespace RE
{
	BoundingBox::BoundingBox()
	{

	}

	BoundingBox::BoundingBox(const VML::VECTOR3F& min, const VML::VECTOR3F& max) : min(min), max(max)
	{

	}

	BoundingBox::~BoundingBox()
	{
		
	}

	bool BoundingBox::Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const
	{
		VML::VECTOR3F o = ray.GetOrigin().asVector3();
		VML::VECTOR3F d = ray.GetDirection().asVector3();

		Vector3<D64> tMin, tMax;

		D64 a = 1.0 / d.x;
		if (a >= 0)
		{
			tMin.X = (min.x - o.x) * a;
			tMax.X = (max.x - o.x) * a;
		}
		else
		{
			tMin.X = (max.x - o.x) * a;
			tMax.X = (min.x - o.x) * a;
		}

		D64 b = 1.0 / d.y;
		if (b >= 0)
		{
			tMin.Y = (min.y - o.y) * b;
			tMax.Y = (max.y - o.y) * b;
		}
		else
		{
			tMin.Y = (max.y - o.y) * b;
			tMax.Y = (min.y - o.y) * b;
		}

		D64 c = 1.0 / d.z;
		if (c >= 0)
		{
			tMin.Z = (min.z - o.z) * c;
			tMax.Z = (max.z - o.z) * c;
		}
		else
		{
			tMin.Z = (max.z - o.z) * c;
			tMax.Z = (min.z - o.z) * c;
		}


		D64 t0, t1;

		// Find largest entering t value
		t0 = std::max(tMin.X, std::max(tMin.Y, tMin.Z));

		// Find smallest exiting t value
		t1 = std::min(tMax.X, std::min(tMax.Y, tMax.Z));

		if (t0 < t1 && t1 > VML::FLOAT_EPSILON)
		{
			RayIntersection r0;
			r0.t = t0;
			r0.worldCoords = ray.GetPointAlongRay(t0);

			RayIntersection r1;
			r1.t = t1;
			r1.worldCoords = ray.GetPointAlongRay(t1);

			if (t0 >= 0.0f)
			{
				outHitInfo.closestIntersection = r0;
				outHitInfo.intersections.push_back(r0);
				outHitInfo.numIntersections++;
			}
			else
				outHitInfo.closestIntersection = r1;

			outHitInfo.intersections.push_back(r1);
			outHitInfo.numIntersections++;

			return true;
		}

		return false;
	}

	bool BoundingBox::Intersects(RayIntersectionList& outHitInfo, const Ray& ray, D64& t0, Vector3<D64>& tMin, Vector3<D64>& tMax)const
	{
		VML::VECTOR3F o = ray.GetOrigin().asVector3();
		VML::VECTOR3F d = ray.GetDirection().asVector3();

		D64 a = 1.0 / d.x;
		if (a >= 0)
		{
			tMin.X = (min.x - o.x) * a;
			tMax.X = (max.x - o.x) * a;
		}
		else
		{
			tMin.X = (max.x - o.x) * a;
			tMax.X = (min.x - o.x) * a;
		}

		D64 b = 1.0 / d.y;
		if (b >= 0)
		{
			tMin.Y = (min.y - o.y) * b;
			tMax.Y = (max.y - o.y) * b;
		}
		else
		{
			tMin.Y = (max.y - o.y) * b;
			tMax.Y = (min.y - o.y) * b;
		}

		D64 c = 1.0 / d.z;
		if (c >= 0)
		{
			tMin.Z = (min.z - o.z) * c;
			tMax.Z = (max.z - o.z) * c;
		}
		else
		{
			tMin.Z = (max.z - o.z) * c;
			tMax.Z = (min.z - o.z) * c;
		}

		D64 t1;

		// Find largest entering t value
		t0 = std::max(tMin.X, std::max(tMin.Y, tMin.Z));

		// Find smallest exiting t value
		t1 = std::min(tMax.X, std::min(tMax.Y, tMax.Z));

		if (t0 < t1 && t1 > VML::FLOAT_EPSILON)
		{
			RayIntersection r0;
			r0.t = t0;
			r0.worldCoords = ray.GetPointAlongRay(t0);

			RayIntersection r1;
			r1.t = t1;
			r1.worldCoords = ray.GetPointAlongRay(t1);

			if (t0 >= 0.0f)
			{
				outHitInfo.closestIntersection = r0;
				outHitInfo.intersections.push_back(r0);
				outHitInfo.numIntersections++;
			}
			else
				outHitInfo.closestIntersection = r1;

			outHitInfo.intersections.push_back(r1);
			outHitInfo.numIntersections++;

			return true;
		}

		return false;
	}

	bool BoundingBox::IsInside(const VML::Vector& point)const
	{
		VML::VECTOR3F p = point.asVector3();

		return p.x >= min.x && p.x <= max.x
			&& p.y >= min.y && p.y <= max.y
			&& p.z >= min.z && p.z <= max.z;
	}
}