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
	
	Color Material::Shade(const ElementIntersection& ei, World& world)
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
		else if (type == "phong")
		{
			F32 ka, kd, ks, exp;
			Color cd, cs;
			iss >> ka >> kd >> cd.r >> cd.g >> cd.b >> cd.a
				>> ks >> cs.r >> cs.g >> cs.b >> cs.a >> exp;

			return new Phong(ka, kd, cd, ks, cs, exp);
		}

		return nullptr;
	}
}