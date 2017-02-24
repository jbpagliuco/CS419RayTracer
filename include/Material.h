#pragma once

#include <Util.h>
#include <Color.h>
#include <BRDF.h>

namespace RE
{
	// Forward declaration
	class World;

	class Material
	{
	public:
		Material();

		virtual ~Material();

		virtual Color Shade(const ElementIntersection& ei, const World& world);
	};

	class Matte : public Material
	{
	public:
		// Creates a matte material.
		// @param ka - The ambient factor.
		// @param kd - The diffuse factor.
		// @param cd - The diffuse color.
		Matte(F32 ka, F32 kd, const Color& cd);

		virtual ~Matte();

		virtual Color Shade(const ElementIntersection& ei, const World& world)override;

	private:
		Lambertian ambientBRDF;
		Lambertian diffuseBRDF;
	};


	class Phong : public Material
	{
	public:
		// Creates a matte material.
		// @param ka - The ambient factor.
		// @param kd - The diffuse factor.
		// @param cd - The diffuse color.
		// @param ks - The specular factor.
		// @param cs - The specular color.
		// @param exp - The specular exponent.
		Phong(F32 ka, F32 kd, const Color& cd, F32 ks, const Color& cs, F32 exp);

		virtual ~Phong();

		virtual Color Shade(const ElementIntersection& ei, const World& world)override;

	private:
		Lambertian ambientBRDF;
		Lambertian diffuseBRDF;
		GlossySpecular specularBRDF;
	};

	Material * CreateMaterialFromString(const std::string& desc);
}