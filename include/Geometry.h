#pragma once

#include <VML.h>
#include "Util.h"
#include <Transform.h>
#include <map>
#include <BoundingVolume.h>
#include <Grid.h>
#include <Ray.h>
#include <Sampler.h>

namespace RE
{
	struct ElementIntersection;

	class Geometry
	{
	public:
		virtual ~Geometry() = default;

		// Checks to see if a ray intersects this geometry.
		// @param [out] outHitInfo - A list of the intersection points.
		// @param ray - The ray to check against.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const = 0;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] t - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& t, const Ray& ray)const = 0;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const = 0;

		virtual F32 PDF(const ElementIntersection& ei)const;

		virtual VML::Vector Sample();

		virtual VML::Vector GetNormalAtPoint(const VML::Vector& p)const;
		
		// Can this object be bounding? (ie planes cannot)
		virtual bool HasBounds()const;
	};






	struct GeometryGridType
	{
		Geometry * pGeo;
		Transform transform;
	};

	struct GeometryIntersection
	{
		GeometryIntersection() : bHit(false) {}

		bool bHit;
		RayIntersectionList rl;
	};

	class GeometryGrid : public RegularGrid<GeometryGridType, GeometryIntersection>
	{
	public:
		GeometryGrid() = default;
		GeometryGrid(F32 m);

		virtual ~GeometryGrid();

	protected:
		// Adds an object to the grid.
		// @param object - The object to add.
		virtual void AddObject(const Object& object) override;

		// Places an object in its cell.
		// @param pObject - A pointer to the object to place.
		virtual void PlaceObject(PObject pObject) override;

		// Trace a ray through the objects and do the necessary calculations.
		// @param [out] out - The output.
		// @param ray - The ray to trace.
		// @param objects - The objects to test intersections.
		// @return { bHit, t }
		virtual ObjectIntersectionOutput CheckObjectIntersections(GeometryIntersection& out, const Ray& ray, const PObjects objects)const override;

		// Trace a ray through the objects and and check for any intersections.
		// @param ray - The ray to trace.
		// @param d - The max distance for an intersection.
		// @param objects - The objects to test intersections.
		// @return Did the ray hit anything?
		virtual bool CheckShallowObjectIntersections(const Ray& ray, F32 d, const PObjects objects)const override;

		// Called after traversing through the grid.
		// @param [out] out - The output.
		// @param ray - The ray to trace.
		virtual void PostTraverse(GeometryIntersection& out, const Ray& ray)const override;

		// Called after traversing through the grid.
		// @param ray - The ray to trace.
		// @return Did the ray hit anything?
		virtual bool PostTraverseShallow(const Ray& ray, F32 d)const override;
	};





	class Plane : public Geometry
	{
	public:
		// Creates a normal of (0, 0, 1).
		Plane();

		// Creates a plane.
		// @param normal - The normal of the plane.
		Plane(const VML::VECTOR3F& normal);

		// Creates a plane.
		// @param normal - The normal of the plane.
		Plane(const VML::Vector& normal);

		// Default destructor.
		virtual ~Plane() = default;

		// Checks to see if a ray intersects this geometry.
		// @param [out] outHitInfo - A list of the intersection points.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] tmin - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& tmin, const Ray& ray)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;

		// Can this object be bounding? (ie planes cannot)
		virtual bool HasBounds()const;

	protected:
		VML::Vector normal;
	};

	class Sphere : public Geometry
	{
	public:
		// Creates a unit sphere.
		Sphere();
		
		// Creates a sphere.
		// @param radius - The radius of the sphere.
		Sphere(F32 radius);

		// Default destructor.
		virtual ~Sphere() = default;

		// Checks to see if a ray interects this sphere.
		// @param outHitInfo - Describes the point of intersection.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this sphere?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const override;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] tmin - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& tmin, const Ray& ray)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;

	protected:
		F32 radius;
	};

	class Triangle : public Geometry
	{
	public:
		Triangle();

		Triangle(VML::VECTOR3F p1, VML::VECTOR3F p2, VML::VECTOR3F p3);

		// Checks to see if a ray interects this sphere.
		// @param outHitInfo - Describes the point of intersection.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this sphere?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const override;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] tmin - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& tmin, const Ray& ray)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;

	private:
		VML::VECTOR3F p1, p2, p3;
		VML::Vector normal;
	};

	RE_ALIGN_MS(16) class Disk : public Geometry
	{
	public:
		Disk(F32 radius, VML::VECTOR3F normal);

		// Checks to see if a ray interects this geometry.
		// @param outHitInfo - Describes the point of intersection.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const override;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] tmin - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& tmin, const Ray& ray)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;

	protected:
		VML::Vector normal;
		F32 radiusSq;
	} RE_ALIGN_GCC(16);

	class Rectangle : public Geometry
	{
	public:
		// Creates a rectangle.
		Rectangle();

		// Default destructor.
		virtual ~Rectangle() = default;

		// Checks to see if a ray interects this sphere.
		// @param outHitInfo - Describes the point of intersection.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this sphere?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const override;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] tmin - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& tmin, const Ray& ray)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;

		virtual F32 PDF(const ElementIntersection& ei)const;

		virtual VML::Vector Sample();

	private:
		Sampler sampler;
		F32 invArea;
	};

	class UGrid : public Geometry
	{
	public:
		// Creates a uniform grid with the provided mesh.
		// @param pGeo - A pointer to the geometry to use.
		// @param n - The extent of the grid.
		// @param scalar - Scalar factor.
		UGrid(Geometry * pGeo, Vector3<I32> n, F32 scalar);

		virtual ~UGrid();

		// Checks to see if a ray intersects this geometry.
		// @param [out] outHitInfo - A list of the intersection points.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const override;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] t - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& t, const Ray& ray)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;
	
	private:
		GeometryGrid grid;
		std::vector<GeometryGridType> objects;
	};






	



	class World;
	Geometry * LoadGeometry(const std::map<std::string, std::string>& params, const World* world);
	Geometry * LoadGeometry(const std::string& params, const World* world);
}