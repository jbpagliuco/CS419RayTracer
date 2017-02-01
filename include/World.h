#pragma once

#include <vector>
#include <Geometry.h>
#include <Material.h>
#include <Light.h>

namespace RE
{
	class Camera;

	struct RayTraceOutput
	{
		Color color;
		F32 depth;
	};

	
	class WorldElement
	{
	public:
		WorldElement(const std::string& name);
		WorldElement(const WorldElement& other);
		WorldElement(WorldElement&& other);

		~WorldElement();

		WorldElement& operator=(const WorldElement& other);
		WorldElement& operator=(WorldElement&& other);


	private:
		void Copy(const WorldElement& other);
		void Destroy();

	public:
		std::string name;

		Geometry* pGeometry;
		Material* pMaterial;
	};




	struct ElementIntersection
	{
		ElementIntersection() : element(nullptr) { }

		WorldElement * element;
		RayIntersection rayInt;
	};



	class World
	{
	public:
		World();

		void AddElement(WorldElement* pElement);
		void DestroyWorld();

		const std::vector<WorldElement*>& GetWorldElements()const;

		void SetCamera(Camera * pCamera);
		Camera * GetCamera()const;

		void AddLight(Light * pLight);
		const std::vector<Light*>& GetLights()const;

		// Traces a ray throughout the world.
		// @param [out] outTraceResult - The result of the ray trace.
		// @param ray - The ray to trace.
		// @return Did the ray hit any objects?
		bool TraceRay(RayTraceOutput& outTraceResult, const Ray& ray)const;

	private:
		void CheckRayElementIntersections(ElementIntersection& out, const Ray& ray)const;
		Color CalculateColor(const ElementIntersection& e)const;

	private:
		Camera * pCamera;
		std::vector<WorldElement*> elements;
		std::vector<Light*> lights;
	};

	void LoadWorldFromFile(World& outWorld, const std::string& file);
}