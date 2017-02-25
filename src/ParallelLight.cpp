#include <Light.h>

namespace RE
{
	ParallelLight::ParallelLight()
	{

	}

	ParallelLight::ParallelLight(F32 ls, Color color, VML::Vector direction) : ls(ls), color(color)
	{
		this->direction = direction.v3Normalize();
	}

	ParallelLight::~ParallelLight()
	{

	}

	F32 ParallelLight::GetDistanceFromPoint(const VML::Vector& point)const
	{
		return std::numeric_limits<F32>::infinity();
	}

	VML::Vector ParallelLight::GetDirectionFromPoint(const VML::Vector& point)const
	{
		return direction;
	}

	Color ParallelLight::CalculateRadiance(const ElementIntersection& ei, const World& world)
	{
		return color * ls;
	}
}