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






	Material * LoadMaterial(const std::map<std::string, std::string>& params)
	{
		std::string type = params.at("type");
		
		std::stringstream ss(params.at("params"));

		if (type == "matte")
		{
			F32 ka, kd;
			Color color;
			ss >> ka >> kd >> color.r >> color.g >> color.b >> color.a;

			return new Matte(ka, kd, color);
		}
		else if (type == "phong")
		{
			F32 ka, kd, ks, exp;
			Color cd, cs;
			ss >> ka >> kd >> cd.r >> cd.g >> cd.b >> cd.a
				>> ks >> cs.r >> cs.g >> cs.b >> cs.a >> exp;

			return new Phong(ka, kd, cd, ks, cs, exp);
		}

		return nullptr;
	}


	Material * LoadMaterial(const std::string& params)
	{
		std::stringstream ss(params);

		std::map<std::string, std::string> p;
		ss >> p["type"];

		std::string curr;
		while (!(ss >> curr).eof())
			p["params"] += curr + " ";
		p["params"] += curr;

		return LoadMaterial(p);
	}
}