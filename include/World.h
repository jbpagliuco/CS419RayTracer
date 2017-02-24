#pragma once

#include <vector>
#include <Geometry.h>
#include <Material.h>
#include <Light.h>
#include <Transform.h>
#include <AssetManager.h>

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
	class WorldElement
	{
	public:
		// Constructor.
		// @param name - The name of the world element.
		WorldElement(const std::string& name);
		
		// Copy constructor
		WorldElement(const WorldElement& other);
		// Move constructor.
		WorldElement(WorldElement&& other);

		// Destructor.
		~WorldElement();

		// Assignment operator.
		WorldElement& operator=(const WorldElement& other);
		WorldElement& operator=(WorldElement&& other);


	private:
		void Copy(const WorldElement& other);
		void Destroy();

	public:
		std::string name;

		Transform transform;
		Geometry* pGeometry;
		Material* pMaterial;
	};



	// Output of a ray trace intersection
	struct ElementIntersection
	{
		ElementIntersection() : element(nullptr) { }

		WorldElement * element;
		RayIntersection rayInt;
	};


	// Holds the world.
	class World
	{
	public:
		// Constructor.
		World();

		// Adds an element to the world.
		// @param pElement - A pointer to the element to add.
		void AddElement(WorldElement* pElement);
		// Destroys the world.
		void DestroyWorld();

		// Gets a list of all the elements in the world.
		const std::vector<WorldElement*>& GetWorldElements()const;

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
		bool TraceRay(RayTraceOutput& outTraceResult, const Ray& ray)const;

	private:
		void CheckRayElementIntersections(ElementIntersection& out, const Ray& ray)const;
		Color CalculateColor(const ElementIntersection& e)const;

	public:
		AssetManager assetManager;
		
	private:
		Camera * pCamera;
		std::vector<WorldElement*> elements;
		std::vector<Light*> lights;
	};

	// Loads a world from a file.
	// @param [out] outWorld - A reference to the world.
	// @param file - The name of the world desc file.
	void LoadWorldFromFile(World& outWorld, const std::string& file);
}