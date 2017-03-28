#include <Geometry.h>

#include <algorithm>

namespace RE
{
	GeometryGrid::GeometryGrid(F32 m) : RegularGrid(m)
	{

	}

	GeometryGrid::~GeometryGrid()
	{

	}

	void GeometryGrid::AddObject(const Object& object)
	{
		BoundingBox bb = object.pGeo->GetBoundingBox();
		VML::VECTOR3F pos = object.transform.position.asVector3();

		// Update min coordinate
		bounds.min.x = std::min(bounds.min.x, bb.min.x + pos.x);
		bounds.min.y = std::min(bounds.min.y, bb.min.y + pos.y);
		bounds.min.z = std::min(bounds.min.z, bb.min.z + pos.z);

		// Update max coordinate
		bounds.max.x = std::max(bounds.max.x, bb.max.x + pos.x);
		bounds.max.y = std::max(bounds.max.y, bb.max.y + pos.y);
		bounds.max.z = std::max(bounds.max.z, bb.max.z + pos.z);
	}

	void GeometryGrid::PlaceObject(PObject pObject)
	{
		BoundingBox bb = pObject->pGeo->GetBoundingBox();
		VML::VECTOR3F pos = pObject->transform.position.asVector3();

		// Min and max cell values (x, y, z)
		Vector3<I32> cMin = CalculateCell(VML::VECTOR3F(bb.min.x + pos.x, bb.min.y + pos.y, bb.min.z + pos.z));
		Vector3<I32> cMax = CalculateCell(VML::VECTOR3F(bb.max.x + pos.x, bb.max.y + pos.y, bb.max.z + pos.z));

		PlaceBBoxInCells(cMin, cMax, pObject);
	}

	GeometryGrid::ObjectIntersectionOutput GeometryGrid::CheckObjectIntersections(GeometryIntersection& out, const Ray& ray, const PObjects objects)const
	{
		// Keep track of the closest element
		F32 minDepth = F32_MAX;
		ObjectIntersectionOutput oiOut;

		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			PObject p = *it;

			RayIntersectionList hitInfo;
			bool bHit = p->pGeo->Intersects(hitInfo, p->transform.TransformRay(ray));
			if (bHit)
			{
				RayIntersection r = hitInfo.closestIntersection;

				// Depth test
				if (r.t < minDepth)
				{
					oiOut.bHit = true;
					oiOut.t = r.t;

					out.bHit = true;
					out.rl = hitInfo;
					minDepth = r.t;
				}
			}
		}

		// The world grid will revrse any normals.

		return oiOut;
	}

	bool GeometryGrid::CheckShallowObjectIntersections(const Ray& ray, F32 d, const PObjects objects)const
	{
		// Keep track of the closest element
		F32 minDepth = F32_MAX;

		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			PObject p = *it;

			F32 t;
			bool bHit = p->pGeo->Intersects(t, p->transform.TransformRay(ray));
			if (bHit && t < d)
				return true;
		}

		return false;
	}

	void GeometryGrid::PostTraverse(GeometryIntersection& out, const Ray& ray)const
	{
	}

	bool GeometryGrid::PostTraverseShallow(const Ray& ray, F32 d)const
	{
		return false;
	}
}