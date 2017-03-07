#include <Grid.h>

#include <World.h>
#include <algorithm>

namespace RE
{
	RegularGrid::RegularGrid()
	{
		unboundedElements = std::vector<const WorldElement*>();
	}

	RegularGrid::~RegularGrid()
	{

	}


	void RegularGrid::AddObjects(const std::vector<WorldElement>& worldElements)
	{
		VML::VECTOR3F min(F32_MAX, F32_MAX, F32_MAX), max(F32_NMAX, F32_NMAX, F32_NMAX);

		// Calculate the bounding box
		for (auto it = worldElements.begin(); it != worldElements.end(); it++)
		{
			WorldElement we = *it;
			
			// Planes, etc don't have bounding boxes
			if (we.pGeometry->HasBounds())
			{
				BoundingBox bb = we.pGeometry->GetBoundingBox();
				VML::VECTOR3F pos = we.transform.position.asVector3();

				// Update min coordinate
				min.x = std::min(min.x, bb.min.x + pos.x);
				min.y = std::min(min.y, bb.min.y + pos.y);
				min.z = std::min(min.z, bb.min.z + pos.z);

				// Update max coordinate
				max.x = std::max(max.x, bb.max.x + pos.x);
				max.y = std::max(max.y, bb.max.y + pos.y);
				max.z = std::max(max.z, bb.max.z + pos.z);
			}
		}

		// Make bounding box slightly larger than all elements
		min.x -= VML::FLOAT_EPSILON; min.y -= VML::FLOAT_EPSILON; min.z -= VML::FLOAT_EPSILON;
		max.x += VML::FLOAT_EPSILON; max.y += VML::FLOAT_EPSILON; max.z += VML::FLOAT_EPSILON;

		bounds = BoundingBox(min, max);

		// Place all the objects in their cells
		PlaceObjects(worldElements);
	}


	void RegularGrid::PlaceObjects(const std::vector<WorldElement>& worldElements)
	{
		VML::VECTOR3F min = bounds.min, max = bounds.max;

		VML::VECTOR3F w(max.x - min.x, max.y - min.y, max.z - min.z);
		F32 m = 2.0f;
		F32 s = (F32)pow(w.x * w.y * w.z / worldElements.size(), 1.0 / 3.0);
		n = Vector3<U32>((U32)(m * w.x / s) + 1, (U32)(m * w.y / s) + 1, (U32)(m * w.z / s) + 1);

		for (auto it = worldElements.begin(); it != worldElements.end(); it++)
		{
			const WorldElement * pWE = &(*it);
			if (pWE->pGeometry->HasBounds())
			{
				BoundingBox bb = pWE->pGeometry->GetBoundingBox();
				VML::VECTOR3F pos = pWE->transform.position.asVector3();

				// Min and max cell values (x,y,z)
				Vector3<I32> cMin(
					Clamp((I32)((bb.min.x + pos.x - min.x) * n.X / (max.x - min.x)), 0, (I32)n.X - 1),
					Clamp((I32)((bb.min.y + pos.y - min.y) * n.Y / (max.y - min.y)), 0, (I32)n.Y - 1),
					Clamp((I32)((bb.min.z + pos.z - min.z) * n.Z / (max.z - min.z)), 0, (I32)n.Z - 1));
				Vector3<I32> cMax(
					Clamp((I32)((bb.max.x + pos.x - min.x) * n.X / (max.x - min.x)), 0, (I32)n.X - 1),
					Clamp((I32)((bb.max.y + pos.y - min.y) * n.Y / (max.y - min.y)), 0, (I32)n.Y - 1),
					Clamp((I32)((bb.max.z + pos.z - min.z) * n.Z / (max.z - min.z)), 0, (I32)n.Z - 1));

				for (I32 z = cMin.Z; z <= cMax.Z; z++)
				{
					for (I32 y = cMin.Y; y <= cMax.Y; y++)
					{
						for (I32 x = cMin.X; x <= cMax.X; x++)
						{
							I32 index = x + n.X * y + n.X * n.Y * z;
							AddElementToCell(index, pWE);
						} // x
					} // y
				} // z
			} // if (bounds)
			else
			{
				unboundedElements.push_back(pWE);
			}
		}
	}

	void RegularGrid::AddElementToCell(I32 idx, const WorldElement * pWE)
	{
		if (cells.find(idx) == cells.end())
			cells[idx] = std::vector<const WorldElement*>();

		cells[idx].push_back(pWE);
	}
}