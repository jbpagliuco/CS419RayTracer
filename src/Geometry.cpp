#include <Geometry.h>

#include <sstream>
#include <P4Mem.h>

namespace RE
{
	Geometry * CreateGeometryFromString(const std::string& desc)
	{
		std::istringstream iss(desc);

		std::string garbage, type;
		iss >> garbage >> type;

		if (type == "sphere")
		{
			std::string oX, oY, oZ, radius;
			iss >> oX >> oY >> oZ >> radius;

			VML::VECTOR3F origin(std::stof(oX), std::stof(oY), std::stof(oZ));
			void *pAlignedMem = P4::AllocateAlignedMemory(sizeof(Sphere), 16);
			return new(pAlignedMem)Sphere(origin, std::stof(radius));
		}
		else if (type == "plane")
		{
			std::string oX, oY, oZ, normalX, normalY, normalZ;
			iss >> oX >> oY >> oZ >> normalX >> normalY >> normalZ;

			VML::VECTOR3F origin(std::stof(oX), std::stof(oY), std::stof(oZ));
			VML::VECTOR3F normal(std::stof(normalX), std::stof(normalY), std::stof(normalZ));
			void *pAlignedMem = P4::AllocateAlignedMemory(sizeof(Plane), 16);
			return new(pAlignedMem)Plane(origin, normal);
		}
		else if (type == "tri")
		{
			VML::VECTOR3F p1, p2, p3;

			iss >> p1.x >> p1.y >> p1.z >> 
				p2.x >> p2.y >> p2.z >> 
				p3.x >> p3.y >> p3.z;

			void *pAlignedMem = P4::AllocateAlignedMemory(sizeof(Triangle), 16);
			return new(pAlignedMem)Triangle(p1, p2, p3);
		}

		return nullptr;
	}
}