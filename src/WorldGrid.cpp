#include <World.h>

#include <algorithm>

namespace RE
{
	WorldGrid::WorldGrid()
	{

	}

	WorldGrid::~WorldGrid()
	{

	}

	void WorldGrid::AddObject(const Renderable& object)
	{
		// Planes, etc don't have bounding boxes
		if (object.pGeometry->HasBounds())
		{
			BoundingBox bb = object.pGeometry->GetBoundingBox();
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
	}

	void WorldGrid::PlaceObject(const Renderable* pObject)
	{
		if (pObject->pGeometry->HasBounds())
		{
			BoundingBox bb = pObject->pGeometry->GetBoundingBox();
			VML::VECTOR3F pos = pObject->transform.position.asVector3();

			// Min and max cell values (x, y, z)
			Vector3<I32> cMin = CalculateCell(VML::VECTOR3F(bb.min.x + pos.x, bb.min.y + pos.y, bb.min.z + pos.z));
			Vector3<I32> cMax = CalculateCell(VML::VECTOR3F(bb.max.x + pos.x, bb.max.y + pos.y, bb.max.z + pos.z));

			PlaceBBoxInCells(cMin, cMax, pObject);
		}
		else
		{
			unboundedElements.push_back(pObject);
		}
	}

	WorldGrid::ObjectIntersectionOutput WorldGrid::CheckObjectIntersections(ElementIntersection& out, const Ray& ray, const PObjects objects)const
	{
		// Keep track of the closest element
		F32 minDepth = out.bHit ? out.rayInt.t : F32_MAX;
		VML::Vector wo = ray.GetDirection();
		wo.negate();

		ObjectIntersectionOutput oiOut;

		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			const Renderable * pWE = *it;

			RayIntersectionList hitInfo;
			bool bHit = pWE->pGeometry->Intersects(hitInfo, pWE->transform.TransformRay(ray));
			if (bHit)
			{
				RayIntersection r = hitInfo.closestIntersection;

				// Depth test
				F32 newDepth = r.t;
				if (newDepth < minDepth)
				{
					oiOut.bHit = true;
					oiOut.t = r.t;

					out.bHit = true;
					out.element = *pWE;
					out.rayInt = r;
					out.ray = ray;
					minDepth = newDepth;
				}
			}
		}

		// Check if we have to reverse the normal
		if (out.rayInt.normal.v3Dot(wo) < 0.0f)
			out.rayInt.normal.negate();

		return oiOut;
	}

	bool WorldGrid::CheckShallowObjectIntersections(const Ray& ray, F32 d, const PObjects objects)const
	{
		// Keep track of the closest element
		F32 minDepth = F32_MAX;
		VML::Vector wo = ray.GetDirection();
		wo.negate();

		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			const Renderable* pWE = *it;

			F32 t;
			bool bHit = pWE->pGeometry->Intersects(t, pWE->transform.TransformRay(ray));
			if (bHit && t < d)
				return true;
		}

		return false;
	}

	void WorldGrid::PostTraverse(ElementIntersection& out, const Ray& ray)const
	{
		CheckObjectIntersections(out, ray, unboundedElements);
	}

	bool WorldGrid::PostTraverseShallow(const Ray& ray, F32 d)const
	{
		return CheckShallowObjectIntersections(ray, d, unboundedElements);
	}
}