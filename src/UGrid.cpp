#include <Geometry.h>

#include <algorithm>

namespace RE
{
	UGrid::UGrid(Geometry * pGeo, Vector3<I32> n, F32 scalar) : grid(1.0f)
	{
		BoundingBox bb = pGeo->GetBoundingBox();
		Vector3<F32> e(bb.max.x - bb.min.x, bb.max.y - bb.min.y, bb.max.z - bb.min.z);

		for (I32 x = -n.X; x <= n.X; x++)
		{
			for (I32 y = -n.Y; y <= n.Y; y++)
			{
				for (I32 z = -n.Z; z <= n.Z; z++)
				{
					VML::VECTOR3F v(x * e.X / scalar, y * e.Y / scalar, z * e.Z / scalar);
					Transform transform(v);
					GeometryGridType g;
					g.pGeo = pGeo;
					g.transform = transform;

					objects.push_back(g);
				}
			}
		}

		grid.AddObjects(objects);
	}

	UGrid::~UGrid()
	{

	}


	bool UGrid::Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const
	{
		GeometryIntersection g = grid.Traverse(ray);
		
		outHitInfo = g.rl;
		return g.bHit;
	}

	bool UGrid::Intersects(F32& t, const Ray& ray)const
	{
		return grid.TraverseShallow(ray, F32_MAX);
	}


	BoundingBox UGrid::GetBoundingBox()const
	{
		return grid.GetBounds();
	}
}