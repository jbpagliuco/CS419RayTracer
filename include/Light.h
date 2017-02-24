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

		// Destructor.
		virtual ~Light();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const = 0;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)const = 0;

		// Calculate the radiance at the incident point.
		// @param ei - The incident point.
		virtual Color CalculateRadiance(const ElementIntersection& ei)const = 0;
	};


	// Ambient light
	RE_ALIGN_MS(16) class AmbientLight : public Light
	{
	public:
		AmbientLight();
		// @param ls - The multiplicative factor.
		// @param color - The color of this light.
		AmbientLight(F32 ls, Color color);

		virtual ~AmbientLight();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)const;

		// Calculate the radiance at the incident point.
		// @param ei - The incident point.
		virtual Color CalculateRadiance(const ElementIntersection& ei)const override;

	private:
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
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)const;

		// Calculate the radiance at the incident point.
		// @param ei - The incident point.
		virtual Color CalculateRadiance(const ElementIntersection& ei)const override;

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
		PointLight(F32 ls, Color color, VML::Vector position);

		virtual ~PointLight();

		// Returns the distance from the point to this light.
		virtual F32 GetDistanceFromPoint(const VML::Vector& point)const;
		// Returns the direction from the point to this light.
		virtual VML::Vector GetDirectionFromPoint(const VML::Vector& point)const;

		// Calculate the radiance at the incident point.
		// @param ei - The incident point.
		virtual Color CalculateRadiance(const ElementIntersection& ei)const override;
	
	protected:
		VML::Vector position;
		Color color;
		F32 ls;
	} RE_ALIGN_GCC(16);
}