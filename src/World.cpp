#include <World.h>

#include <Log.h>
#include <P4Mem.h>
#include <Camera.h>
#include <algorithm>

namespace RE
{
	World::World()
	{
		elements = std::vector<WorldElement*>();
		lights = std::vector<Light*>();
	}

	void World::AddElement(WorldElement* pElement)
	{
		elements.push_back(pElement);
	}

	void World::DestroyWorld()
	{
		RE_LOG(ENGINE, END, "Destroying world");

		// Destroy elements
		for (auto it = elements.begin(); it != elements.end(); it++)
		{
			WorldElement * p = *it;
			delete p;
		}

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

	const std::vector<WorldElement*>& World::GetWorldElements()const
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





	bool World::TraceRay(RayTraceOutput& outTraceResult, const Ray& ray)const
	{
		ElementIntersection closest;
		CheckRayElementIntersections(closest, ray);

		if (closest.element)
		{
			outTraceResult.color = CalculateColor(closest);

			return true;
		}

		return false;
	}




	void World::CheckRayElementIntersections(ElementIntersection& out, const Ray& ray)const
	{
		// Keep track of the closest element
		F32 minDepth = 100000000.0f;
		VML::Vector wo = ray.GetDirection();
		wo.negate();

		for (auto it = elements.begin(); it != elements.end(); it++)
		{
			WorldElement* e = *it;

			RayIntersectionList hitInfo;
			bool bHit = e->pGeometry->Intersects(hitInfo, ray, e->transform);
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

					out.element = e;
					out.rayInt = r;
					out.ray = ray;
					minDepth = newDepth;
				}
			}
		}
	}


	Color World::CalculateColor(const ElementIntersection& e)const
	{
		WorldElement *pElement = e.element;
		Material *pMat = e.element->pMaterial;

		return pMat->Shade(e, *this);
	}
}