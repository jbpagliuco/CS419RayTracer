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
		for (auto it = geometry.unnamed.begin(); it != geometry.unnamed.end(); it++)
		{
			Geometry *p = *it;
			p->~Geometry();
			P4::FreeAlignedMemory(p);
		}

		for (auto it = geometry.named.begin(); it != geometry.named.end(); it++)
		{
			Geometry *p = it->second;
			p->~Geometry();
			P4::FreeAlignedMemory(p);
		}


		// Free materials
		for (auto it = materials.unnamed.begin(); it != materials.unnamed.end(); it++)
		{
			Material *p = *it;
			delete p;
		}

		for (auto it = materials.named.begin(); it != materials.named.end(); it++)
		{
			Material *p = it->second;
			delete p;
		}

		// Free textures
		for (auto it = tex2ds.unnamed.begin(); it != tex2ds.unnamed.end(); it++)
		{
			Texture2D *p = *it;
			delete p;
		}

		for (auto it = tex2ds.named.begin(); it != tex2ds.named.end(); it++)
		{
			Texture2D *p = it->second;
			delete p;
		}
	}


	void AssetManager::AddGeometry(Geometry* pGeo)
	{
		geometry.unnamed.push_back(pGeo);
	}

	void AssetManager::AddGeometry(const std::string& name, Geometry* pGeo)
	{
		if (!pGeo)
			return;

		geometry.named[name] = pGeo;
	}

	Geometry* AssetManager::GetGeometry(const std::string& name)const
	{
		assert(geometry.named.find(name) != geometry.named.end());

		return geometry.named.at(name);
	}

	void AssetManager::AddMaterial(Material* pMat)
	{
		materials.unnamed.push_back(pMat);
	}

	void AssetManager::AddMaterial(const std::string& name, Material* pMat)
	{
		if (!pMat)
			return;

		materials.named[name] = pMat;
	}

	Material* AssetManager::GetMaterial(const std::string& name)const
	{
		assert(materials.named.find(name) != materials.named.end());

		return materials.named.at(name);
	}





	void AssetManager::AddTexture2D(Texture2D* pTex)
	{
		tex2ds.unnamed.push_back(pTex);
	}

	void AssetManager::AddTexture2D(const std::string& name, Texture2D* pTex)
	{
		if (!pTex)
			return;

		tex2ds.named[name] = pTex;
	}

	Texture2D* AssetManager::GetTexture2D(const std::string& name)const
	{
		assert(tex2ds.named.find(name) != tex2ds.named.end());

		return tex2ds.named.at(name);
	}
}