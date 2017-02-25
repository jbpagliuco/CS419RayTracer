#include <Geometry.h>

#include <sstream>
#include <P4Mem.h>

namespace RE
{
	Geometry * LoadGeometry(const std::map<std::string, std::string>& params)
	{
		std::string type = params.at("type");
		std::stringstream ss(params.at("params"));

		if (type == "sphere")
		{
			F32 radius;
			ss >> radius;

			void *pAlignedMem = P4::AllocateAlignedMemory(sizeof(Sphere), 16);
			return new(pAlignedMem)Sphere(radius);
		}
		else if (type == "plane")
		{
			VML::VECTOR3F normal;
			ss >> normal.x >> normal.y >> normal.z;

			void *pAlignedMem = P4::AllocateAlignedMemory(sizeof(Plane), 16);
			return new(pAlignedMem)Plane(normal);
		}
		else if (type == "tri")
		{
			VML::VECTOR3F p1, p2, p3;

			ss >> p1.x >> p1.y >> p1.z >> 
				p2.x >> p2.y >> p2.z >> 
				p3.x >> p3.y >> p3.z;

			void *pAlignedMem = P4::AllocateAlignedMemory(sizeof(Triangle), 16);
			return new(pAlignedMem)Triangle(p1, p2, p3);
		}

		return nullptr;
	}

	Geometry * LoadGeometry(const std::string& params)
	{
		std::stringstream ss(params);

		std::map<std::string, std::string> p;
		ss >> p["type"];

		std::string curr;
		while (!(ss >> curr).eof())
			p["params"] += curr + " ";
		p["params"] += curr;

		return LoadGeometry(p);
	}
}