#include <BRDF.h>

// For ElementIntersection
#include <World.h>

namespace RE
{
	Lambertian::Lambertian(F32 kd, const Color& cd) : kd(kd), cd(cd)
	{
		
	}

	Lambertian::~Lambertian()
	{

	}

	Color Lambertian::F(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const
	{
		return cd * kd * INV_PI;
	}

	Color Lambertian::SampleF(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const
	{
		return Color();
	}

	Color Lambertian::Rho(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const
	{
		return cd * kd;
	}

	Color Lambertian::Rho(const ElementIntersection& ei, const VML::Vector& wo)const
	{
		return cd * kd;
	}
}