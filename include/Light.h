#pragma once

#include <Util.h>
#include <Material.h>
#include <VML.h>

namespace RE
{
	struct ElementIntersection;

	class Light
	{
	public:
		Light();
		Light(Color diffuse, Color specular);

		virtual ~Light();

		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const = 0;
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)const = 0;

		virtual Color CalculatePhongLighting(const ElementIntersection& e, const VML::Vector& eyeDir)const = 0;

	protected:
		Color diffuse;
		Color specular;
	};


	class ParallelLight : public Light
	{
	public:
		ParallelLight();
		ParallelLight(Color diffuse, Color specular, VML::Vector direction);
		
		virtual ~ParallelLight();

		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const;
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)const;

		virtual Color CalculatePhongLighting(const ElementIntersection& e, const VML::Vector& eyeDir)const;

	protected:
		VML::Vector direction;
	};


	class PointLight : public Light
	{
	public:
		PointLight();
		PointLight(Color diffuse, Color specular, VML::Vector position, F32 rangeSquared);

		virtual ~PointLight();

		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const;
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)const;

		virtual Color CalculatePhongLighting(const ElementIntersection& e, const VML::Vector& eyeDir)const;
	
	protected:
		VML::Vector position;
		F32 rangeSquared;
	};
}