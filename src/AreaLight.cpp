#include <Light.h>

#include <World.h>

namespace RE
{
	AreaLight::AreaLight(Renderable * pElement, bool bCastsShadows) :
		pElement(pElement), Light(bCastsShadows)
	{

	}

	AreaLight::~AreaLight()
	{

	}

	F32 AreaLight::GetDistanceFromPoint(const VML::Vector& point)const
	{
		return (pElement->transform.position - point).v3Length();
	}

	VML::Vector AreaLight::GetDirectionFromPoint(const VML::Vector& point)
	{
		samplePoint = pElement->pGeometry->Sample();
		normal = pElement->pGeometry->GetNormalAtPoint(samplePoint);
		wi = (samplePoint - point).v3Normalize();

		return wi;
	}

	Color AreaLight::CalculateRadiance(const ElementIntersection& ei, const World& world)
	{
		F32 nDOTd = normal.negate().v3Dot(wi);

		if (nDOTd > 0.0f)
			return ((Emissive*)(pElement->pMaterial))->GetLe();
		else
			return Color();
	}

	F32 AreaLight::G(const ElementIntersection& ei)const
	{
		VML::Vector nNormal = VML::Vector(normal).negate();
		F32 nDOTd = nNormal.v3Dot(wi);
		F32 d2 = (ei.rayInt.worldCoords - samplePoint).v3LengthSq();
		
		return nDOTd / d2;
	}

	F32 AreaLight::PDF(const ElementIntersection& ei)const
	{
		return pElement->pGeometry->PDF(ei);
	}
}