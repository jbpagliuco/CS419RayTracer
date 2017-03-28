#include <Light.h>

#include <World.h>

namespace RE
{
	PointLight::PointLight()
	{

	}

	PointLight::PointLight(F32 ls, Color color, VML::Vector position, bool bCastsShadows) : ls(ls), color(color), Light(bCastsShadows)
	{
		this->position = position;
	}

	PointLight::~PointLight()
	{

	}


	F32 PointLight::GetDistanceFromPoint(const VML::Vector& point)const
	{
		return (position - point).v3Length();
	}

	VML::Vector PointLight::GetDirectionFromPoint(const VML::Vector& point)
	{
		return (position - point).v3Normalize();
	}



	Color PointLight::CalculateRadiance(const ElementIntersection& ei, const World& world)
	{
		return color * ls;
	}
}