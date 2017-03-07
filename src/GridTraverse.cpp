#include <Grid.h>

#include <World.h>

namespace RE
{
	bool RegularGrid::GetStartingCell(Vector3<I32>& c, const Ray& ray, D64& t0, Vector3<D64>& tMin, Vector3<D64>& tMax)const
	{
		RayIntersectionList hitList;
		bool bHit = bounds.Intersects(hitList, ray, t0, tMin, tMax);

		if (!bHit)
			return false;

		VML::Vector p;
		if (bounds.IsInside(ray.GetOrigin()))
			p = ray.GetOrigin();
		else
			p = ray.GetPointAlongRay((F32)t0);

		c.X = Clamp((I32)((p.getX() - bounds.min.x) * n.X / (bounds.max.x - bounds.min.x)), 0, (I32)n.X - 1);
		c.Y = Clamp((I32)((p.getY() - bounds.min.y) * n.Y / (bounds.max.y - bounds.min.y)), 0, (I32)n.Y - 1);
		c.Z = Clamp((I32)((p.getZ() - bounds.min.z) * n.Z / (bounds.max.z - bounds.min.z)), 0, (I32)n.Z - 1);

		return true;
	}




	void RegularGrid::CheckElementIntersections(ElementIntersection& out, const Ray& ray, const std::vector<const WorldElement*> elements)const
	{
		// Keep track of the closest element
		F32 minDepth = out.bHit ? out.rayInt.t : 100000000.0f;
		VML::Vector wo = ray.GetDirection();
		wo.negate();

		for (auto it = elements.begin(); it != elements.end(); it++)
		{
			const WorldElement * pWE = *it;

			RayIntersectionList hitInfo;
			bool bHit = pWE->pGeometry->Intersects(hitInfo, ray, pWE->transform);
			if (bHit)
			{
				RayIntersection r = hitInfo.closestIntersection;

				// Depth test
				F32 newDepth = r.t;
				if (newDepth < minDepth)
				{
					// Check if we have to reverse the normal
					if (r.normal.v3Dot(wo) < 0.0f)
						r.normal.negate();

					out.bHit = true;
					out.element = *pWE;
					out.rayInt = r;
					out.ray = ray;
					minDepth = newDepth;
				}
			}
		}
	}

	void RegularGrid::Traverse(ElementIntersection& out, const Ray& ray)const
	{
		Vector3<I32> i;
		D64 t0;
		Vector3<D64> tMin, tMax;
		bool bHit = GetStartingCell(i, ray, t0, tMin, tMax);

		Vector3<F32> d((F32)(tMax.X - tMin.X) / n.X, (F32)(tMax.Y - tMin.Y) / n.Y, (F32)(tMax.Z - tMin.Z) / n.Z);
		Vector3<F32> tNext((F32)(t0 + (i.X + 1) * d.X), (F32)(t0 + (i.Y + 1) * d.Y), (F32)(t0 + (i.Z + 1) * d.Z));

		while (bHit)
		{
			// This cell's linear index
			I32 l = i.X + n.X * i.Y + n.X * n.Y * i.Z;
			if (cells.find(l) == cells.end())
				out.bHit = false;
			else
				CheckElementIntersections(out, ray, cells.at(l));

			if (tNext.X < tNext.Y && tNext.X < tNext.Z)
			{
				if (out.bHit && (out.rayInt.t < tNext.X))
					return;

				tNext.X += d.X;
				i.X++;

				if (i.X == n.X)
					break;
			}
			else if (tNext.Y < tNext.Z)
			{
				if (out.bHit && (out.rayInt.t < tNext.Y))
					return;

				tNext.Y += d.Y;
				i.Y++;

				if (i.Y == n.Y)
					break;
			}
			else
			{
				if (out.bHit && (out.rayInt.t < tNext.Z))
					return;

				tNext.Z += d.Z;
				i.Z++;

				if (i.Z == n.Z)
					break;
			}
		}

		// Check unbounded geometry
		CheckElementIntersections(out, ray, unboundedElements);
	}




	void RegularGrid::CheckElementIntersections(bool& bHit, F32 d, const Ray& ray, const std::vector<const WorldElement*> elements)const
	{
		// Keep track of the closest element
		F32 minDepth = 100000000.0f;
		VML::Vector wo = ray.GetDirection();
		wo.negate();

		for (auto it = elements.begin(); it != elements.end(); it++)
		{
			const WorldElement* pWE = *it;

			F32 t;
			bHit = pWE->pGeometry->Intersects(t, ray, pWE->transform);
			if (bHit && t < d)
				return;
		}

		bHit = false;
	}
	
	void RegularGrid::Traverse(bool& bHit, F32 d, const Ray& ray)const
	{
		Vector3<I32> i;
		D64 t0;
		Vector3<D64> tMin, tMax;
		bool bHitGrid = GetStartingCell(i, ray, t0, tMin, tMax);

		Vector3<F32> dT((F32)(tMax.X - tMin.X) / n.X, (F32)(tMax.Y - tMin.Y) / n.Y, (F32)(tMax.Z - tMax.Z) / n.Z);
		Vector3<F32> tNext(tMin.X + (i.X + 1) * dT.X, tMin.Y + (i.Y + 1) * dT.Y, tMin.Z + (i.Z + 1) * dT.Z);
		Vector3<I32> iStep(1, 1, 1), iStop(n.X, n.Y, n.Z);

		while (bHitGrid)
		{
			// This cell's linear index
			I32 l = i.X + n.X * i.Y + n.X * n.Y * i.Z;
			if (cells.find(l) == cells.end())
				bHit = false;
			else
				CheckElementIntersections(bHit, d, ray, cells.at(l));

			if (bHit)
				return;

			if (tNext.X < tNext.Y && tNext.X < tNext.Z)
			{
				tNext.X += dT.X;
				i.X += iStep.X;

				if (i.X == iStop.X)
				{
					bHit = false;
					return;
				}
			}
			else if (tNext.Y < tNext.Z)
			{
				tNext.Y += dT.Y;
				i.Y += iStep.Y;

				if (i.Y == iStop.Y)
				{
					bHit = false;
					return;
				}
			}
			else
			{
				tNext.Z += dT.Z;
				i.Z += iStep.Z;

				if (i.Z == iStop.Z)
				{
					bHit = false;
					return;
				}
			}
		}

		// We've hit nothing, so check unbounded geometry.
		CheckElementIntersections(bHit, d, ray, unboundedElements);
	}
}