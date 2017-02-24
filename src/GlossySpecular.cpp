#include <BRDF.h>

// For ElementIntersection
#include <World.h>

namespace RE
{
	GlossySpecular::GlossySpecular(const Color& cs, F32 ks, F32 exp) : cs(cs), ks(ks), exp(exp)
	{

	}

	GlossySpecular::~GlossySpecular()
	{

	}

	Color GlossySpecular::F(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const
	{
		Color color;
		VML::Vector nWi = wi;
		nWi.negate();

		F32 ndotwi = ei.rayInt.normal.v3Dot(wi);
		VML::Vector r(nWi + ei.rayInt.normal * 2.0f * ndotwi);
		F32 rdotwo = r.v3Dot(wo);

		if (rdotwo > 0.0f)
			color = cs * ks * pow(rdotwo, exp);

		return color;
	}

	Color GlossySpecular::SampleF(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const
	{
		return Color();
	}

	Color GlossySpecular::Rho(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const
	{
		return Color();
	}

	Color GlossySpecular::Rho(const ElementIntersection& ei, const VML::Vector& wo)const
	{
		return Color();
	}
}