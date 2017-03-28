#pragma once

#include <vector>
#include <Geometry.h>
#include <Material.h>
#include <Light.h>
#include <Transform.h>
#include <AssetManager.h>
#include <Color.h>
#include <Grid.h>
#include <KDTree.h>
#include <Renderable.h>

namespace RE
{
	// Forward declaration
	class Camera;

	// Output of tracing a ray through the world.
	struct RayTraceOutput
	{
		Color color;
		F32 depth;
	};
	

	// Output of a ray trace intersection
	struct ElementIntersection
	{
		ElementIntersection() : bHit(false) { }

		bool bHit;
		Renderable element;
		RayIntersection rayInt;
		Ray ray;
	};







	class WorldGrid : public RegularGrid<Renderable, ElementIntersection>
	{
	public:
		WorldGrid();

		virtual ~WorldGrid();

	protected:
		// Adds an object to the grid.
		// @param object - The object to add.
		virtual void AddObject(const Renderable& object) override;

		// Places an object in its cell.
		// @param pObject - A pointer to the object to place.
		virtual void PlaceObject(const Renderable* pObject) override;

		// Trace a ray through the objects and do the necessary calculations.
		// @param [out] out - The output.
		// @param ray - The ray to trace.
		// @param objects - The objects to test intersections.
		// @return { bHit, t }
		virtual ObjectIntersectionOutput CheckObjectIntersections(ElementIntersection& out, const Ray& ray, const PObjects objects)const override;

		// Trace a ray through the objects and and check for any intersections.
		// @param ray - The ray to trace.
		// @param d - The max distance for an intersection.
		// @param objects - The objects to test intersections.
		// @return Did the ray hit anything?
		virtual bool CheckShallowObjectIntersections(const Ray& ray, F32 d, const PObjects objects)const override;

		// Called after traversing through the grid.
		// @param [out] out - The output.
		// @param ray - The ray to trace.
		virtual void PostTraverse(ElementIntersection& out, const Ray& ray)const override;

		// Called after traversing through the grid.
		// @param ray - The ray to trace.
		// @return Did the ray hit anything?
		virtual bool PostTraverseShallow(const Ray& ray, F32 d)const override;

	private:
		// Elements that can't be bounded (ie planes), thus can't be part of the grid
		PObjects unboundedElements;
	};






	class KDTypeWorldElement : public KDType
	{
	public:
		KDTypeWorldElement() = default;
		KDTypeWorldElement(Renderable e);

		virtual ~KDTypeWorldElement();

		virtual Geometry* GetGeometry()const override;
		virtual Transform GetTransform()const override;
		virtual BoundingBox GetBoundingBox()const override;

	private:
		Renderable e;

		friend class World;
	};


	// Holds the world.
	class World
	{
	public:
		// Constructor.
		World();

		void LoadFromFile(const std::string& file);

		// Adds an element to the world.
		// @param element - The element to add.
		void AddElement(const Renderable& element);
		// Destroys the world.
		void DestroyWorld();

		// Gets a list of all the elements in the world.
		const std::vector<KDTypeWorldElement>& GetRenderables()const;

		// Sets the camera to render from.
		// @param pCamera - A pointer to the camera.
		void SetCamera(Camera * pCamera);
		// Gets the world's camera.
		Camera * GetCamera()const;

		// Adds a light to the world.
		// @param pLight - A pointer to the light.
		void AddLight(Light * pLight);
		// Gets a list of all the lights in the world.
		const std::vector<Light*>& GetLights()const;

		// Traces a ray throughout the world.
		// @param [out] outTraceResult - The result of the ray trace.
		// @param ray - The ray to trace.
		// @return Did the ray hit any objects?
		bool TraceRay(RayTraceOutput& outTraceResult, const Ray& ray);

		void CheckRayElementIntersections(ElementIntersection& out, const Ray& ray)const;
		void CheckRayElementIntersections(bool& bHit, F32 d, const Ray& ray)const;

	private:
		Color CalculateColor(const ElementIntersection& e);

	public:
		AssetManager assetManager;
		AmbientOccluder ambientLight;

	private:
		Camera * pCamera;
		std::vector<KDTypeWorldElement> elements;
		std::vector<Light*> lights;

		KDTree<KDTypeWorldElement> tree;
	};

}