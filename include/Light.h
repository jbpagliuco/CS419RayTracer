#pragma once

#include <Util.h>
#include <Material.h>
#include <VML.h>
#include <Geometry.h>
#include <Renderable.h>

namespace RE
{
	struct ElementIntersection;

	class Light
	{
	public:
		// Constructor.
		Light(bool bCastsShadows = true);

		// Destructor.
		virtual ~Light();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const = 0;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point) = 0;

		// Calculate the radiance at the incident point.
		// @param ei - The incident point.
		virtual Color CalculateRadiance(const ElementIntersection& ei, const World& world) = 0;

		// Is this element in the shadow of this light?
		// @param ray - The ray.
		// @param ei - The intersection point.
		virtual bool InShadow(const Ray& ray, const ElementIntersection& ei, const World& world)const;

		virtual F32 G(const ElementIntersection& ei)const;

		virtual F32 PDF(const ElementIntersection& ei)const;

	protected:
		bool bCastsShadows;
	};


	// Ambient light
	class AmbientLight : public Light
	{
	public:
		AmbientLight();
		// @param ls - The multiplicative factor.
		// @param color - The color of this light.
		AmbientLight(F32 ls, Color color);

		virtual ~AmbientLight();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const override;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)override;

		// Calculate the radiance at the incident point.
		// @param ei - The incident point.
		virtual Color CalculateRadiance(const ElementIntersection& ei, const World& world)override;

		// Is this element in the shadow of this light?
		// @param ray - The ray.
		// @param ei - The intersection point.
		virtual bool InShadow(const Ray& ray, const ElementIntersection& ei, const World& world)const override;

	private:
		Color color;
		F32 ls;
	};


	// Ambient light
	RE_ALIGN_MS(16) class AmbientOccluder : public Light
	{
	public:
		AmbientOccluder();
		// @param ls - The multiplicative factor.
		// @param color - The color of this light.
		// @param gen - The sampler generator function.
		// @param minAmount - The 
		AmbientOccluder(F32 ls, Color color, SamplerGenerator gen, U32 numSamples, F32 minAmount);

		virtual ~AmbientOccluder();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const override;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)override;

		// Calculate the radiance at the incident point.
		// @param ei - The incident point.
		virtual Color CalculateRadiance(const ElementIntersection& ei, const World& world)override;

	private:
		virtual VML::Vector SampleDirection();

	private:
		VML::Vector u, v, w;
		Sampler sampler;
		F32 minAmount;
		Color color;
		F32 ls;
	} RE_ALIGN_GCC(16);



	// Light with a direction, but no position (ie sun).
	RE_ALIGN_MS(16) class ParallelLight : public Light
	{
	public:
		ParallelLight();
		// Creates a parallel light.
		// @param ls - The multiplicative factor.
		// @param color - The color of this light.
		// @param direction - The direction for this light.
		ParallelLight(F32 ls, Color color, VML::Vector direction);
		
		virtual ~ParallelLight();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const override;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)override;

		// Calculate the radiance at the incident point.
		// @param ei - The incident point.
		virtual Color CalculateRadiance(const ElementIntersection& ei, const World& world)override;

	protected:
		VML::Vector direction;
		Color color;
		F32 ls;
	} RE_ALIGN_GCC(16);


	// A light with a position and range.
	RE_ALIGN_MS(16) class PointLight : public Light
	{
	public:
		PointLight();
		// Creates a point light.
		// @param ls - The multiplicative factor.
		// @param color - The color of this light.
		// @param position - The position of this light.
		PointLight(F32 ls, Color color, VML::Vector position, bool bCastsShadows);

		virtual ~PointLight();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const override;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)override;

		// Calculate the radiance at the incident point.
		// @param ei - The incident point.
		virtual Color CalculateRadiance(const ElementIntersection& ei, const World& world)override;
	
	protected:
		VML::Vector position;
		Color color;
		F32 ls;
	} RE_ALIGN_GCC(16);





	// A light with a finite area.
	RE_ALIGN_MS(16) class AreaLight : public Light
	{
	public:
		// Creates an area light.
		// @param ls - The multiplicative factor.
		// @param color - The color of this light.
		// @param position - The position of this light.
		AreaLight(Renderable * pElement, bool bCastsShadows);

		virtual ~AreaLight();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const override;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)override;

		// Calculate the radiance at the incident point.
		// @param ei - The incident point.
		virtual Color CalculateRadiance(const ElementIntersection& ei, const World& world)override;

		virtual F32 G(const ElementIntersection& ei)const override;

		virtual F32 PDF(const ElementIntersection& ei)const override;

	protected:
		Renderable * pElement;
		VML::Vector normal, samplePoint, wi;
	} RE_ALIGN_GCC(16);
}