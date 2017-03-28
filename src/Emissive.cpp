#include <Material.h>
#include <World.h>

namespace RE
{
	Emissive::Emissive(Color c, F32 ls) : c(c), ls(ls)
	{

	}

	Emissive::~Emissive()
	{

	}

	Color Emissive::Shade(const ElementIntersection& ei, World& world)
	{
		VML::Vector nNormal = ei.rayInt.normal;
		nNormal.negate();
		if (nNormal.v3Dot(ei.ray.GetDirection()) > 0.0f)
			return c * ls;
		else
			return Color();
	}

	Color Emissive::AreaLightShade(const ElementIntersection& ei, World& world)
	{
		VML::Vector nNormal = ei.rayInt.normal;
		nNormal.negate();
		if (nNormal.v3Dot(ei.ray.GetDirection()) > 0.0f)
			return c * ls;
		else
			return Color();
	}

	Color Emissive::GetLe()const
	{
		return c * ls;
	}
}