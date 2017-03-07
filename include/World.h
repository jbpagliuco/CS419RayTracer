#pragma once

#include <vector>
#include <Geometry.h>
#include <Material.h>
#include <Light.h>
#include <Transform.h>
#include <AssetManager.h>
#include <Color.h>
#include <Grid.h>

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


	// A renderable element (sphere, tri, etc)
	struct WorldElement
	{
		WorldElement() {}
		// Constructor.
		// @param name - The name of the world element.
		WorldElement(const std::string& name) : name(name) {}

	public:
		std::string name;

		Transform transform;
		Geometry* pGeometry;
		Material* pMaterial;
	};
	

	// Output of a ray trace intersection
	struct ElementIntersection
	{
		ElementIntersection() : bHit(false) { }

		bool bHit;
		WorldElement element;
		RayIntersection rayInt;
		Ray ray;
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
		void AddElement(const WorldElement& element);
		// Destroys the world.
		void DestroyWorld();

		// Gets a list of all the elements in the world.
		const std::vector<WorldElement>& GetWorldElements()const;

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
		std::vector<WorldElement> elements;
		std::vector<Light*> lights;

		RegularGrid grid;
	};

}