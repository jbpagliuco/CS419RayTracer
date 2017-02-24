#pragma once

#include <Sampler.h>
#include <Color.h>

namespace RE
{
	// Forward declaration
	struct ElementIntersection;

	class BRDF
	{
	public:
		BRDF();

		virtual ~BRDF();

		// Calculates the BRDF.
		// @param ei - The intersection.
		// @param wi - The incoming direction.
		// @param wo - The outgoing direction.
		virtual Color F(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const = 0;

		// Samples the BRDF.
		// @param ei - The intersection.
		// @param wi - The incoming direction.
		// @param wo - The outgoing direction.
		virtual Color SampleF(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const = 0;

		// Calculates the bihemispherical reflectance.
		// @param ei - The intersection.
		// @param wi - The incoming direction.
		// @param wo - The outgoing direction.
		virtual Color Rho(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const = 0;

		// Calculates the bihemispherical reflectance.
		// @param ei - The intersection.
		// @param wo - The outgoing direction.
		virtual Color Rho(const ElementIntersection& ei, const VML::Vector& wo)const = 0;

	protected:
		Sampler sampler;
	};


	class Lambertian : public BRDF
	{
	public:
		Lambertian(F32 kd, const Color& cd);

		virtual ~Lambertian();

		// Calculates the BRDF.
		// @param ei - The intersection.
		// @param wi - The incoming direction.
		// @param wo - The outgoing direction.
		virtual Color F(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const override;

		// Samples the BRDF.
		// @param ei - The intersection.
		// @param wi - The incoming direction.
		// @param wo - The outgoing direction.
		virtual Color SampleF(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const override;

		// Calculates the bihemispherical reflectance.
		// @param ei - The intersection.
		// @param wi - The incoming direction.
		// @param wo - The outgoing direction.
		virtual Color Rho(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const override;

		// Calculates the bihemispherical reflectance.
		// @param ei - The intersection.
		// @param wo - The outgoing direction.
		virtual Color Rho(const ElementIntersection& ei, const VML::Vector& wo)const override;

	private:
		// Diffuse multiplier
		F32 kd;
		// Diffuse color
		Color cd;
	};


	class GlossySpecular : public BRDF
	{
	public:
		GlossySpecular(const Color& cs, F32 ks, F32 exp);

		virtual ~GlossySpecular();

		// Calculates the BRDF.
		// @param ei - The intersection.
		// @param wi - The incoming direction.
		// @param wo - The outgoing direction.
		virtual Color F(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const override;

		// Samples the BRDF.
		// @param ei - The intersection.
		// @param wi - The incoming direction.
		// @param wo - The outgoing direction.
		virtual Color SampleF(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const override;

		// Calculates the bihemispherical reflectance.
		// @param ei - The intersection.
		// @param wi - The incoming direction.
		// @param wo - The outgoing direction.
		virtual Color Rho(const ElementIntersection& ei, const VML::Vector& wi, const VML::Vector& wo)const override;

		// Calculates the bihemispherical reflectance.
		// @param ei - The intersection.
		// @param wo - The outgoing direction.
		virtual Color Rho(const ElementIntersection& ei, const VML::Vector& wo)const override;

	private:
		// Specular color
		Color cs;
		// Specular multiplier
		F32 ks;
		// Specular exponent
		F32 exp;
	};
}