#include <Light.h>

#include <World.h>
#include <algorithm>

namespace RE
{
	Light::Light()
	{

	}

	Light::Light(Color diffuse, Color specular)
	{
		this->diffuse = diffuse;
		this->specular = specular;
	}

	Light::~Light()
	{

	}







	ParallelLight::ParallelLight()
	{

	}

	ParallelLight::ParallelLight(Color diffuse, Color specular, VML::Vector direction) : Light(diffuse, specular)
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

	Color ParallelLight::CalculatePhongLighting(const ElementIntersection& e, const VML::Vector& eyeDir)const
	{
		Material * pMat = e.element->pMaterial;
		F32 diffuseDot = std::max(direction.v3Dot(e.rayInt.normal), 0.0f);
		Color d = pMat->GetDiffuseColor() * diffuseDot * diffuse;

		VML::Vector r = e.rayInt.normal * (2 * direction.v3Dot(e.rayInt.normal)) - direction;
		F32 specularDot = std::max(r.v3Dot(eyeDir), 0.0f);
		Color s = pMat->GetSpecularColor() * pow(specularDot, pMat->GetShininess()) * specular;

		return d + s;
	}










	PointLight::PointLight()
	{

	}

	PointLight::PointLight(Color diffuse, Color specular, VML::Vector position, F32 rangeSquared) : Light(diffuse, specular)
	{
		this->position = position;
		this->rangeSquared = rangeSquared;
	}

	PointLight::~PointLight()
	{

	}


	F32 PointLight::GetDistanceFromPoint(const VML::Vector& point)const
	{
		return (position - point).v3Length();
	}

	VML::Vector PointLight::GetDirectionFromPoint(const VML::Vector& point)const
	{
		return (position - point).v3Normalize();
	}

	Color PointLight::CalculatePhongLighting(const ElementIntersection& e, const VML::Vector& eyeDir)const
	{
		VML::Vector direction = position - e.rayInt.worldCoords;

		if (direction.v3LengthSq() <= rangeSquared)
		{
			direction.v3Normalize();

			Material * pMat = e.element->pMaterial;
			F32 diffuseDot = std::max(direction.v3Dot(e.rayInt.normal), 0.0f);
			Color d = pMat->GetDiffuseColor() * diffuseDot * diffuse;

			VML::Vector r = e.rayInt.normal * (2 * direction.v3Dot(e.rayInt.normal)) - direction;
			F32 specularDot = std::max(r.v3Dot(eyeDir), 0.0f);
			Color s = pMat->GetSpecularColor() * pow(specularDot, pMat->GetShininess()) * specular;

			return d + s;
		}

		return Color();
	}
}