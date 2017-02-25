#include <AssetManager.h>
#include <P4Mem.h>
#include <assert.h>

namespace RE
{
	AssetManager::AssetManager()
	{
	}

	AssetManager::~AssetManager()
	{
		// Free geometry
		for (auto it = unnamedGeometry.begin(); it != unnamedGeometry.end(); it++)
		{
			Geometry *p = *it;
			P4::FreeAlignedMemory(p);
		}

		for (auto it = geometry.begin(); it != geometry.end(); it++)
		{
			Geometry *p = it->second;
			P4::FreeAlignedMemory(p);
		}


		// Free materials
		for (auto it = unnamedMaterials.begin(); it != unnamedMaterials.end(); it++)
		{
			Material *p = *it;
			delete p;
		}

		for (auto it = materials.begin(); it != materials.end(); it++)
		{
			Material *p = it->second;
			delete p;
		}
	}


	void AssetManager::AddGeometry(Geometry* pGeo)
	{
		unnamedGeometry.push_back(pGeo);
	}

	void AssetManager::AddGeometry(const std::string& name, Geometry* pGeo)
	{
		if (!pGeo)
			return;

		geometry[name] = pGeo;
	}

	Geometry* AssetManager::GetGeometry(const std::string& name)
	{
		assert(geometry.find(name) != geometry.end());

		return geometry[name];
	}

	void AssetManager::AddMaterial(Material* pMat)
	{
		unnamedMaterials.push_back(pMat);
	}

	void AssetManager::AddMaterial(const std::string& name, Material* pMat)
	{
		if (!pMat)
			return;

		materials[name] = pMat;
	}

	Material* AssetManager::GetMaterial(const std::string& name)
	{
		assert(materials.find(name) != materials.end());

		return materials[name];
	}
}