#include <World.h>

#include <Log.h>
#include <P4Mem.h>
#include <Camera.h>
#include <algorithm>

namespace RE
{
	World::World()
	{
		elements = std::vector<KDTypeWorldElement>();
		lights = std::vector<Light*>();
	}

	void World::AddElement(const Renderable& element)
	{
		elements.push_back(KDTypeWorldElement(element));
	}

	void World::DestroyWorld()
	{
		RE_LOG(ENGINE, END, "Destroying world");

		// Destroy camera
		if (pCamera)
		{
			pCamera->~Camera();
			P4::FreeAlignedMemory(pCamera);
			pCamera = nullptr;
		}

		// Destroy lights
		for (auto it = lights.begin(); it != lights.end(); it++)
		{
			Light * p = *it;

			p->~Light();
			P4::FreeAlignedMemory(p);
		}
	}

	const std::vector<KDTypeWorldElement>& World::GetRenderables()const
	{
		return elements;
	}

	void World::SetCamera(Camera * pCamera)
	{
		this->pCamera = pCamera;
	}

	Camera * World::GetCamera()const
	{
		return pCamera;
	}



	void World::AddLight(Light * pLight)
	{
		lights.push_back(pLight);
	}

	const std::vector<Light*>& World::GetLights()const
	{
		return lights;
	}





	bool World::TraceRay(RayTraceOutput& outTraceResult, const Ray& ray)
	{
		ElementIntersection closest;
		CheckRayElementIntersections(closest, ray);

		if (closest.bHit)
		{
			outTraceResult.color = CalculateColor(closest);

			return true;
		}

		return false;
	}

	void World::CheckRayElementIntersections(ElementIntersection& out, const Ray& ray)const
	{
		RayIntersectionList outHitInfo;
		KDTypeWorldElement hitElement;
		out.bHit = tree.Traverse(outHitInfo, ray, hitElement);
		out.element = hitElement.e;
		out.ray = ray;
		out.rayInt = outHitInfo.closestIntersection;
	}

	void World::CheckRayElementIntersections(bool& bHit, F32 d, const Ray& ray)const
	{
		bHit = tree.TraverseShallow(ray, d);
	}


	Color World::CalculateColor(const ElementIntersection& e)
	{
		Renderable element = e.element;
		Material *pMat = element.pMaterial;

		return pMat->Shade(e, *this);
	}
}