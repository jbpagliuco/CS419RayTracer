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

		for (auto it = elements.begin(); it != elements.end(); it++)
		{
			WorldElement* e = *it;

			RayIntersectionList hitInfo;
			bool bHit = e->pGeometry->Intersects(hitInfo, ray);
			if (bHit)
			{
				RayIntersection r = hitInfo.closestIntersection;

				// Depth test
				F32 newDepth = r.t;
				if (newDepth < minDepth)
				{
					out.element = e;
					out.rayInt = r;
					minDepth = newDepth;
				}
			}
		}
	}


	Color World::CalculateColor(const ElementIntersection& e)const
	{
		Color out;
		VML::Vector eyeDir = (pCamera->GetPosition() - e.rayInt.worldCoords).v3Normalize();
		WorldElement *pElement = e.element;
		Material *pMat = pElement->pMaterial;
		VML::Vector p = e.rayInt.worldCoords;

		for (auto it = lights.begin(); it != lights.end(); it++)
		{
			Light *pLight = *it;

			// Are we in the shadow of this light?
			VML::Vector dirToLight = pLight->GetDirectionFromPoint(p);
			F32 distanceToLight = pLight->GetDistanceFromPoint(p);

			ElementIntersection betweenPointAndLight;
			CheckRayElementIntersections(betweenPointAndLight, Ray(p, dirToLight, F32(RAY_EPSILON)));

			if (betweenPointAndLight.element && betweenPointAndLight.rayInt.t < distanceToLight)
			{
				// We are in a shadow

			}
			else
			{
				// Not in shadow
				out += pLight->CalculatePhongLighting(e, eyeDir);
			}
		}
		
		// Add ambient light
		out += pMat->GetDiffuseColor() * pMat->GetAmbient();

		return out;
	}
}