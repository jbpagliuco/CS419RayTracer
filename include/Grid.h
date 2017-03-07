#pragma once

#include <map>
#include <vector>
#include <Util.h>
#include <VML.h>
#include <Geometry.h>

namespace RE
{
	struct WorldElement;
	struct ElementIntersection;

	class RegularGrid
	{
	public:
		RegularGrid();

		virtual ~RegularGrid();

		void AddObjects(const std::vector<WorldElement>& worldElements);

		void Traverse(ElementIntersection& out, const Ray& ray)const;
		void Traverse(bool& bHit, F32 d, const Ray& ray)const;

	private:
		void PlaceObjects(const std::vector<WorldElement>& worldElements);
		void AddElementToCell(I32 idx, const WorldElement * pWE);

		bool GetStartingCell(Vector3<I32>& c, const Ray& ray, D64& t0, Vector3<D64>& tMin, Vector3<D64>& tMax)const;

		void CheckElementIntersections(ElementIntersection& out, const Ray& ray, const std::vector<const WorldElement*> elements)const;
		void CheckElementIntersections(bool& bHit, F32 d, const Ray& ray, const std::vector<const WorldElement*> elements)const;

	private:
		// Linear cell index -> List of world elements in cell
		std::map<I32, std::vector<const WorldElement*>> cells;

		// For things that can't be put in the grid, like planes
		std::vector<const WorldElement*> unboundedElements;

		BoundingBox bounds;
		Vector3<U32> n;
	};
}