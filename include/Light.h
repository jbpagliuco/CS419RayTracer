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
		// Constructor.
		Light();
		// Creates a light with the diffuse and specular colors.
		Light(Color diffuse, Color specular);

		// Destructor.
		virtual ~Light();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const = 0;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)const = 0;

		// Calculates the lit color, according to the phone shading model.
		virtual Color CalculatePhongLighting(const ElementIntersection& e, const VML::Vector& eyeDir)const = 0;

	protected:
		Color diffuse;
		Color specular;
	};

	// Light with a direction, but no position (Infinite light).
	class ParallelLight : public Light
	{
	public:
		ParallelLight();
		// Creates a parallel light.
		// @param diffuse - The diffuse color.
		// @param specular - The specular color.
		// @param direction - The direction for this light.
		ParallelLight(Color diffuse, Color specular, VML::Vector direction);
		
		virtual ~ParallelLight();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)const;

		// Calculates the lit color, according to the phone shading model.
		virtual Color CalculatePhongLighting(const ElementIntersection& e, const VML::Vector& eyeDir)const;

	protected:
		VML::Vector direction;
	};


	// A light with a position and range.
	class PointLight : public Light
	{
	public:
		PointLight();
		// Creates a point light.
		// @param diffuse - The diffuse color.
		// @param specular - The specular color.
		// @param position - The position of this light.
		// @param rangeSquared - The range of this point light, squared.
		PointLight(Color diffuse, Color specular, VML::Vector position, F32 rangeSquared);

		virtual ~PointLight();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)const;

		// Calculates the lit color, according to the phone shading model.
		virtual Color CalculatePhongLighting(const ElementIntersection& e, const VML::Vector& eyeDir)const;
	
	protected:
		VML::Vector position;
		F32 rangeSquared;
	};
}