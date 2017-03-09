#include <BRDF.h>

// For ElementIntersection
#include <World.h>

namespace RE
{
	SVLambertian::SVLambertian(F32 kd, Texture2D * pTex) : kd(kd), pTex(pTex)
	{

	}

	SVLambertian::~SVLambertian()
	{

	}

	Color SVLambertian::F(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const
	{
		Color cd = pTex->GetColor(ei.rayInt.uv.x, ei.rayInt.uv.y);
		return cd * kd * (F32)INV_PI;
	}

	Color SVLambertian::SampleF(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const
	{
		return Color();
	}

	Color SVLambertian::Rho(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const
	{
		Color cd = pTex->GetColor(ei.rayInt.uv.x, ei.rayInt.uv.y);
		return cd * kd;
	}

	Color SVLambertian::Rho(const ElementIntersection& ei, const VML::Vector& wo)const
	{
		Color cd = pTex->GetColor(ei.rayInt.uv.x, ei.rayInt.uv.y);
		return cd * kd;
	}
}