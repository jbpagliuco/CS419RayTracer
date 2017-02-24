#include <Material.h>

#include <sstream>

#include <World.h>

namespace RE
{
	Material::Material()
	{

	}

	Material::~Material()
	{

	}
	
	Color Material::Shade(const ElementIntersection& ei, const World& world)
	{
		return Color();
	}






	Material * CreateMaterialFromString(const std::string& desc)
	{
		std::istringstream iss(desc);

		std::string type;
		iss >> type >> type;

		if (type == "matte")
		{
			F32 ka, kd;
			Color color;
			iss >> ka >> kd >> color.r >> color.g >> color.b >> color.a;

			return new Matte(ka, kd, color);
		}

		return nullptr;
	}
}