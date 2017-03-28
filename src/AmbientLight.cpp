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

	VML::Vector AmbientLight::GetDirectionFromPoint(const VML::Vector& point)
	{
		return VML::VectorZero();
	}

	Color AmbientLight::CalculateRadiance(const ElementIntersection& ei, const World& world)
	{
		return color * ls;
	}

	bool AmbientLight::InShadow(const Ray& ray, const ElementIntersection& ei, const World& world)const
	{
		return false;
	}
}