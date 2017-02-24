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
}