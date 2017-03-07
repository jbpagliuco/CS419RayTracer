#include <World.h>

#include <Log.h>
#include <P4Mem.h>
#include <Camera.h>
#include <Light.h>
#include <RenderingEngine.h>

#include <WorldFileReader.h>

namespace RE
{
	void LoadGeometry(const std::string& name, TypeParams params, void* pWorld)
	{
		World * p = (World*)pWorld;
		if (name != "")
		{
			Geometry * pGeo = LoadGeometry(params);
			p->assetManager.AddGeometry(name, pGeo);
		}
		else
		{
			
		}
	}

	void LoadMaterial(const std::string& name, TypeParams params, void* pWorld)
	{
		World * p = (World*)pWorld;
		if (name != "")
		{
			Material * pMat = LoadMaterial(params);
			p->assetManager.AddMaterial(name, pMat);
		}
		else
		{

		}
	}

	void LoadWorldElement(const std::string& name, TypeParams params, void* pWorld)
	{
		World * p = (World*)pWorld;
		
		WorldElement e(name);
		e.transform = LoadTransform(params["Transform"]);
		
		std::string geometry = params["Geometry"];
		if (geometry[0] == '@')
			e.pGeometry = p->assetManager.GetGeometry(geometry.substr(1));
		else
		{
			e.pGeometry = LoadGeometry(geometry);
			p->assetManager.AddGeometry(e.pGeometry);
		}

		std::string material = params["Material"];
		if (material[0] == '@')
			e.pMaterial = p->assetManager.GetMaterial(material.substr(1));
		else
		{
			e.pMaterial = LoadMaterial(material);
			p->assetManager.AddMaterial(e.pMaterial);
		}

		p->AddElement(e);
	}

	void LoadCamera(const std::string& name, TypeParams params, void* pWorld)
	{
		World * p = (World*)pWorld;

		VML::VECTOR3F pos, lookat, up;
		
		std::stringstream ss(params["pos"]);
		ss >> pos.x >> pos.y >> pos.z;
		
		ss = std::stringstream(params["lookat"]);
		ss >> lookat.x >> lookat.y >> lookat.z;

		ss = std::stringstream(params["up"]);
		ss >> up.x >> up.y >> up.z;
		
		std::string type;
		ss = std::stringstream(params["type"]);
		ss >> type;

		Camera * pCamera;
		if (type == "ortho")
		{
			F32 halfWidth;
			ss >> halfWidth;

			void * pAlignedMem = P4::AllocateAlignedMemory(sizeof(OrthographicCamera), 16);
			pCamera = new(pAlignedMem)OrthographicCamera(pos, lookat, up, halfWidth);
		}
		else// if (type == "pinhole")
		{
			F32 distanceToViewPlane, pwScale;
			ss >> distanceToViewPlane >> pwScale;

			void * pAlignedMem = P4::AllocateAlignedMemory(sizeof(PinholeCamera), 16);
			pCamera = new(pAlignedMem)PinholeCamera(pos, lookat, up, distanceToViewPlane, pwScale);
		}

		p->SetCamera(pCamera);
	}

	void LoadLight(const std::string& name, TypeParams params, void* pWorld)
	{
		World * p = (World*)pWorld;
		
		std::string type = params["type"];
		std::stringstream ss(params["params"]);
	
		if (type == "ambient")
		{
			F32 ls, minAmount;
			Color color;

			ss >> ls >> color.r >> color.g >> color.b >> minAmount;
			color.a = 1.0f;

			p->ambientLight = AmbientOccluder(ls, color, RE::RenderingEngine::SamplerFunc, RE::RenderingEngine::NumSamples, minAmount);
		}
		else if (type == "parallel")
		{
			F32 ls;
			Color color;
			VML::VECTOR4F direction;

			ss >> ls >>
				color.r >> color.g >> color.b >>
				direction.x >> direction.y >> direction.z;
			color.a = 1.0f;
			direction.w = 0.0f;

			void * pAlignedMem = P4::AllocateAlignedMemory(sizeof(ParallelLight), 16);
			ParallelLight* light = new(pAlignedMem)ParallelLight(ls, color, VML::Vector(direction));
			p->AddLight(light);
		}
		else if (type == "point")
		{
			F32 ls;
			Color color;
			VML::VECTOR3F position;
			std::string castsShadows;

			ss >> ls >>
				color.r >> color.g >> color.b >>
				position.x >> position.y >> position.z >>
				castsShadows;

			void * pAlignedMem = P4::AllocateAlignedMemory(sizeof(PointLight), 16);
			PointLight* light = new(pAlignedMem)PointLight(ls, color, VML::Vector(position), castsShadows == "true");
			p->AddLight(light);
		}
	}





	void World::LoadFromFile(const std::string& file)
	{
		WorldFileReader wfr;

		wfr.AddRule("Geometry", std::vector<std::string>({ "type", "params" }), LoadGeometry);
		wfr.AddRule("Material", std::vector<std::string>({ "type", "params" }), LoadMaterial);
		wfr.AddRule("WorldElement", std::vector<std::string>({ "Transform", "Geometry", "Material" }), LoadWorldElement);
		wfr.AddRule("Camera", std::vector<std::string>({ "pos", "lookat", "up", "type" }), LoadCamera);
		wfr.AddRule("Light", std::vector<std::string>({ "type", "params" }), LoadLight);

		wfr.ReadFromFile(file, this);

		grid.AddObjects(elements);
	}
}