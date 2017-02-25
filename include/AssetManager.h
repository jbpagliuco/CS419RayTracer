#pragma once

#include <Geometry.h>
#include <Material.h>
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
		Geometry* GetGeometry(const std::string& name);

		void AddMaterial(Material* pMat);
		void AddMaterial(const std::string& name, Material* pMat);
		Material* GetMaterial(const std::string& name);

	private:
		std::vector<Geometry*> unnamedGeometry;
		std::map<std::string, Geometry*> geometry;

		std::vector<Material*> unnamedMaterials;
		std::map<std::string, Material*> materials;
	};
}