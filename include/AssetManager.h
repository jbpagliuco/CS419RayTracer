#pragma once

#include <Geometry.h>
#include <Material.h>
#include <Texture.h>
#include <map>

namespace RE
{
	class AssetManager
	{
	public:
		AssetManager();

		virtual ~AssetManager();

		void AddGeometry(Geometry* pGeo);
		void AddGeometry(const std::string& name, Geometry* pGeo);
		Geometry* GetGeometry(const std::string& name)const;

		void AddMaterial(Material* pMat);
		void AddMaterial(const std::string& name, Material* pMat);
		Material* GetMaterial(const std::string& name)const;

		void AddTexture2D(Texture2D* pTex);
		void AddTexture2D(const std::string& name, Texture2D* pTex);
		Texture2D* GetTexture2D(const std::string& name)const;

	private:
		template <typename T>
		struct AssetContainer
		{
			AssetContainer() : unnamed(std::vector<T*>()), named(std::map<std::string, T*>()) {}
			std::vector<T*> unnamed;
			std::map<std::string, T*> named;
		};
		
		AssetContainer<Geometry> geometry;
		AssetContainer<Material> materials;
		AssetContainer<Texture2D> tex2ds;
	};
}