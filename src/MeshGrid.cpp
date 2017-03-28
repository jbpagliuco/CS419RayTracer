#include <Mesh.h>

#include <algorithm>

namespace RE
{
	MeshGrid::MeshGrid()
	{

	}

	MeshGrid::~MeshGrid()
	{

	}

	void MeshGrid::AddObject(const MeshTriangle& object)
	{
		BoundingBox bb = object.GetBoundingBox();

		// Update min coordinate
		bounds.min.x = std::min(bounds.min.x, bb.min.x);
		bounds.min.y = std::min(bounds.min.y, bb.min.y);
		bounds.min.z = std::min(bounds.min.z, bb.min.z);

		// Update max coordinate
		bounds.max.x = std::max(bounds.max.x, bb.max.x);
		bounds.max.y = std::max(bounds.max.y, bb.max.y);
		bounds.max.z = std::max(bounds.max.z, bb.max.z);
	}

	void MeshGrid::PlaceObject(const MeshTriangle* pObject)
	{
		BoundingBox bb = pObject->GetBoundingBox();

		// Min and max cell values (x, y, z)
		Vector3<I32> cMin = CalculateCell(bb.min);
		Vector3<I32> cMax = CalculateCell(bb.max);

		PlaceBBoxInCells(cMin, cMax, pObject);
	}

	MeshGrid::ObjectIntersectionOutput MeshGrid::CheckObjectIntersections(MeshTriangleIntersection& out, const Ray& ray, const PObjects objects)const
	{
		// Keep track of the closest element
		F32 minDepth = F32_MAX;
		ObjectIntersectionOutput oiOut;

		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			const MeshTriangle * p = *it;

			RayIntersectionList hitInfo;
			bool bHit = p->Intersects(hitInfo, ray);
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

	bool MeshGrid::CheckShallowObjectIntersections(const Ray& ray, F32 d, const PObjects objects)const
	{
		// Keep track of the closest element
		F32 minDepth = F32_MAX;

		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			const MeshTriangle * p = *it;

			F32 t;
			bool bHit = p->Intersects(t, ray);
			if (bHit && t < d)
				return true;
		}

		return false;
	}

	void MeshGrid::PostTraverse(MeshTriangleIntersection& out, const Ray& ray)const
	{
	}

	bool MeshGrid::PostTraverseShallow(const Ray& ray, F32 d)const
	{
		return false;
	}
}