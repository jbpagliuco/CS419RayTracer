#pragma once

#include <map>
#include <vector>
#include <Util.h>
#include <VML.h>
#include <Ray.h>
#include <BoundingVolume.h>

namespace RE
{
	template <typename ObjectType, typename OutputType>
	class RegularGrid
	{
	public:
		typedef ObjectType Object;
		typedef std::vector<ObjectType> Objects;
		typedef const ObjectType* PObject;
		typedef std::vector<PObject> PObjects;

	protected:
		struct ObjectIntersectionOutput
		{
			ObjectIntersectionOutput() : bHit(false), t(0.0f) {}
			bool bHit;
			F32 t;
		};

	public:
		RegularGrid();

		virtual ~RegularGrid();

		// Add a list of objects to this grid.
		// @param objects - The original objects.
		virtual void AddObjects(const Objects& objects);

		// Shoot a ray through the grid.
		// @param ray - The ray to trace.
		// @return The output of traversing this ray through the grid.
		virtual OutputType Traverse(const Ray& ray)const;

		// Shallow shoot a ray through the grid. Only checks for object intersection, no other calculations involved.
		// @param ray - The ray to trace.
		// @param d - The maximum distance for instersection.
		// @return Did the ray hit anything?
		virtual bool TraverseShallow(const Ray& ray, F32 d)const;

		// Gets the bounding box of this grid.
		virtual BoundingBox GetBounds()const;

	protected:
		// Adds an object to the grid.
		// @param object - The object to add.
		virtual void AddObject(const Object& object) = 0;

		// Places an object in its cell.
		// @param pObject - A pointer to the object to place.
		virtual void PlaceObject(PObject pObject) = 0;

		// Calculates the cell at the specified point.
		// @param point - The point.
		Vector3<I32> CalculateCell(const VML::VECTOR3F& point)const;

		// Places the object in the cells from min to max.
		// @param min - The minimum cell.
		// @param max - The maximum cell.
		// @param pObject - A pointer to the object to place in the cells.
		void PlaceBBoxInCells(const Vector3<I32>& min, const Vector3<I32>& max, PObject pObject);

		// Trace a ray through the objects and do the necessary calculations.
		// @param [out] out - The output.
		// @param ray - The ray to trace.
		// @param objects - The objects to test intersections.
		// @return { bHit, t }
		virtual ObjectIntersectionOutput CheckObjectIntersections(OutputType& out, const Ray& ray, const PObjects objects)const = 0;

		// Trace a ray through the objects and and check for any intersections.
		// @param ray - The ray to trace.
		// @param d - The max distance for an intersection.
		// @param objects - The objects to test intersections.
		// @return Did the ray hit anything?
		virtual bool CheckShallowObjectIntersections(const Ray& ray, F32 d, const PObjects objects)const = 0;

		// Called after traversing through the grid.
		// @param [out] out - The output.
		// @param ray - The ray to trace.
		virtual void PostTraverse(OutputType& out, const Ray& ray)const = 0;

		// Called after traversing through the grid.
		// @param ray - The ray to trace.
		// @return Did the ray hit anything?
		virtual bool PostTraverseShallow(const Ray& ray, F32 d)const = 0;

	private:
		// Calculates the grid dimensions.
		void CreateGrid(U64 numObjects);

		// Place each object in its cell.
		// @param objects - A list of all the objects to place.
		void PlaceObjects(const Objects& objects);

		// Add an object to the cell map.
		// @param idx - The cell index.
		// @param pObject - A pointer to the object.
		void AddElementToCell(U32 idx, PObject pObject);

		// Gets the starting cell for this ray.
		// @param [out] cell - The cell value.
		// @param [out] t0 - The t value of where the ray hits the grid's box.
		// @param [out] tMin - The minimum t value for this ray against the bounding box.
		// @param [out] tMax - The maximum t value for this ray against the bounding box.
		// @param ray - The ray to trace.
		bool GetStartingCell(Vector3<I32>& cell, D64& t0, Vector3<D64>& tMin, Vector3<D64>& tMax, const Ray& ray)const;

		bool TraverseInit(Vector3<I32>& cell, Vector3<F32>& d, Vector3<F32>& next, Vector3<I32>& step, Vector3<I32>& stop, const Ray& ray)const;

	protected:
		// Linear cell index -> List of world elements in cell
		std::map<I32, PObjects> cells;

		BoundingBox bounds;
		Vector3<I32> n;
	};






















	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	// IMPLEMENTATION //////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////

	template <typename Object, typename OutputType>
	RegularGrid<typename Object, typename OutputType>
		::RegularGrid()
	{

	}

	template <typename Object, typename OutputType>
	RegularGrid<typename Object, typename OutputType>
		::~RegularGrid()
	{
	}



	template <typename Object, typename OutputType>
	void RegularGrid<Object, OutputType>::AddObjects(const Objects& objects)
	{
		bounds = BoundingBox(VML::VECTOR3F(F32_MAX, F32_MAX, F32_MAX), VML::VECTOR3F(F32_NMAX, F32_NMAX, F32_NMAX));

		// Calculate the bounding box
		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			Object o = *it;
			AddObject(o);
		}

		// Make bounding box slightly larger than all elements
		bounds.min.x -= VML::FLOAT_EPSILON; bounds.min.y -= VML::FLOAT_EPSILON; bounds.min.z -= VML::FLOAT_EPSILON;
		bounds.max.x += VML::FLOAT_EPSILON; bounds.max.y += VML::FLOAT_EPSILON; bounds.max.z += VML::FLOAT_EPSILON;

		// Calculate the grid dimensions
		CreateGrid(objects.size());

		// Place all the objects in their cells
		PlaceObjects(objects);
	}


	template <typename Object, typename OutputType>
	void RegularGrid<Object, OutputType>::CreateGrid(U64 numObjects)
	{
		VML::VECTOR3F min = bounds.min, max = bounds.max;

		VML::VECTOR3F w(max.x - min.x, max.y - min.y, max.z - min.z);
		F32 m = 2.0f;
		F32 s = (F32)pow(w.x * w.y * w.z / numObjects, 0.33333333333333);
		n = Vector3<I32>((I32)(m * w.x / s) + 1, (I32)(m * w.y / s) + 1, (I32)(m * w.z / s) + 1);
	}


	template <typename Object, typename OutputType>
	void RegularGrid<Object, OutputType>::PlaceObjects(const Objects& objects)
	{
		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			PObject p = &(*it);
			PlaceObject(p);
		}
	}



	template <typename Object, typename OutputType>
	BoundingBox RegularGrid<Object, OutputType>::GetBounds()const
	{
		return bounds;
	}


	template <typename Object, typename OutputType>
	void RegularGrid<Object, OutputType>::AddElementToCell(U32 idx, PObject pObject)
	{
		if (cells.find(idx) == cells.end())
			cells[idx] = PObjects();

		cells[idx].push_back(pObject);
	}



	template <typename Object, typename OutputType>
	Vector3<I32> RegularGrid<Object, OutputType>::CalculateCell(const VML::VECTOR3F& point)const
	{
		Vector3<I32> cell;
		cell.X = (I32)Clamp((I32)((point.x - bounds.min.x) * n.X / (bounds.max.x - bounds.min.x)), 0, n.X - 1);
		cell.Y = (I32)Clamp((I32)((point.y - bounds.min.y) * n.Y / (bounds.max.y - bounds.min.y)), 0, n.Y - 1);
		cell.Z = (I32)Clamp((I32)((point.z - bounds.min.z) * n.Z / (bounds.max.z - bounds.min.z)), 0, n.Z - 1);

		return cell;
	}


	template <typename Object, typename OutputType>
	void RegularGrid<Object, OutputType>::PlaceBBoxInCells(const Vector3<I32>& min, const Vector3<I32>& max, PObject pObject)
	{
		for (I32 z = min.Z; z <= max.Z; z++)
		{
			for (I32 y = min.Y; y <= max.Y; y++)
			{
				for (I32 x = min.X; x <= max.X; x++)
				{
					I32 index = x + n.X * y + n.X * n.Y * z;
					AddElementToCell(index, pObject);
				}
			}
		}
	}



	template <typename Object, typename OutputType>
	bool RegularGrid<Object, OutputType>::GetStartingCell(Vector3<I32>& cell, D64& t0, Vector3<D64>& tMin, Vector3<D64>& tMax, const Ray& ray)const
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

		cell = CalculateCell(p.asVector3());
		return true;
	}

	template <typename Object, typename OutputType>
	bool RegularGrid<Object, OutputType>::TraverseInit(Vector3<I32>& cell, Vector3<F32>& d, Vector3<F32>& next, Vector3<I32>& step, Vector3<I32>& stop, const Ray& ray)const
	{
		D64 t0;
		Vector3<D64> tMin, tMax;
		bool bHit = GetStartingCell(cell, t0, tMin, tMax, ray);

		if (!bHit)
			return false;

		d = Vector3<F32>((F32)(tMax.X - tMin.X) / n.X, (F32)(tMax.Y - tMin.Y) / n.Y, (F32)(tMax.Z - tMin.Z) / n.Z);
		VML::VECTOR3F dir = ray.GetDirection().asVector3();

		if (VML::FEquals(dir.x, 0.0))
		{
			next.X = F32_MAX;
			step.X = -1;
			stop.X = -1;
		}
		else if (dir.x > 0)
		{
			next.X = (F32)tMin.X + (cell.X + 1) * d.X;
			step.X = +1;
			stop.X = n.X;
		}
		else
		{
			next.X = (F32)tMin.X + (n.X - cell.X) * d.X;
			step.X = -1;
			stop.X = -1;
		}

		if (VML::FEquals(dir.y, 0.0))
		{
			next.Y = F32_MAX;
			step.Y = -1;
			stop.Y = -1;
		}
		else if (dir.y > 0)
		{
			next.Y = (F32)tMin.Y + (cell.Y + 1) * d.Y;
			step.Y = +1;
			stop.Y = n.Y;
		}
		else
		{
			next.Y = (F32)tMin.Y + (n.Y - cell.Y) * d.Y;
			step.Y = -1;
			stop.Y = -1;
		}

		if (VML::FEquals(dir.z, 0.0))
		{
			next.Z = F32_MAX;
			step.Z = -1;
			stop.Z = -1;
		}
		else if (dir.z > 0)
		{
			next.Z = (F32)tMin.Z + (cell.Z + 1) * d.Z;
			step.Z = +1;
			stop.Z = n.Z;
		}
		else
		{
			next.Z = (F32)tMin.Z + (n.Z - cell.Z) * d.Z;
			step.Z = -1;
			stop.Z = -1;
		}

		return true;
	}







	template <typename Object, typename OutputType>
	OutputType RegularGrid<Object, OutputType>::Traverse(const Ray& ray)const
	{
		Vector3<I32> cell;
		Vector3<F32> d, next;
		Vector3<I32> step, stop;
		bool bHit = TraverseInit(cell, d, next, step, stop, ray);

		OutputType out;
		while (bHit)
		{
			// This cell's linear index
			I32 l = cell.X + n.X * cell.Y + n.X * n.Y * cell.Z;
			ObjectIntersectionOutput oiOut;

			if (cells.find(l) != cells.end())
				oiOut = CheckObjectIntersections(out, ray, cells.at(l));
			else
				oiOut.bHit = false;

			if (next.X < next.Y && next.X < next.Z)
			{
				if (oiOut.bHit && (oiOut.t < next.X))
					break;

				next.X += d.X;
				cell.X += step.X;

				if (cell.X == stop.X)
					break;
			}
			else if (next.Y < next.Z)
			{
				if (oiOut.bHit && (oiOut.t < next.Y))
					break;

				next.Y += d.Y;
				cell.Y += step.Y;

				if (cell.Y == stop.Y)
					break;
			}
			else
			{
				if (oiOut.bHit && (oiOut.t < next.Z))
					break;

				next.Z += d.Z;
				cell.Z += step.Z;

				if (cell.Z == stop.Z)
					break;
			}
		}

		PostTraverse(out, ray);

		return out;
	}


	template <typename Object, typename OutputType>
	bool RegularGrid<Object, OutputType>::TraverseShallow(const Ray& ray, F32 dist)const
	{
		Vector3<I32> cell;
		Vector3<F32> d, next;
		Vector3<I32> step, stop;
		bool bHitGrid = TraverseInit(cell, d, next, step, stop, ray);

		bool bHit = false;
		while (bHitGrid)
		{
			// This cell's linear index
			I32 l = cell.X + n.X * cell.Y + n.X * n.Y * cell.Z;

			if (cells.find(l) != cells.end())
				bHit = CheckShallowObjectIntersections(ray, dist, cells.at(l));

			if (bHit)
				return true;

			if (next.X < next.Y && next.X < next.Z)
			{
				next.X += d.X;
				cell.X += step.X;

				if (cell.X == stop.X)
					break;
			}
			else if (next.Y < next.Z)
			{
				next.Y += d.Y;
				cell.Y += step.Y;

				if (cell.Y == stop.Y)
					break;
			}
			else
			{
				next.Z += d.Z;
				cell.Z += step.Z;

				if (cell.Z == stop.Z)
					break;
			}
		}

		return PostTraverseShallow(ray, dist);
	}
}