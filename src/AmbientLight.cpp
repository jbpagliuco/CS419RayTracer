#include <Light.h>

namespace RE
{
	AmbientLight::AmbientLight()
	{

	}

	AmbientLight::AmbientLight(F32 ls, Color color) : ls(ls), color(color)
	{

	}

	AmbientLight::~AmbientLight()
	{

	}
	
	F32 AmbientLight::GetDistanceFromPoint(const VML::Vector& point)const
	{
		return 0.0f;
	}

	VML::Vector AmbientLight::GetDirectionFromPoint(const VML::Vector& point)const
	{
		return VML::VectorZero();
	}

	Color AmbientLight::CalculateRadiance(const ElementIntersection& ei)const
	{
		return color * ls;
	}
}