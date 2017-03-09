#pragma once

#include <Util.h>
#include <Color.h>
#include <BRDF.h>
#include <map>

namespace RE
{
	// Forward declaration
	class World;

	class Material
	{
	public:
		Material();

		virtual ~Material();

		virtual Color Shade(const ElementIntersection& ei, World& world);
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

		virtual Color Shade(const ElementIntersection& ei, World& world)override;

	private:
		Lambertian ambientBRDF;
		Lambertian diffuseBRDF;
	};



	class SVMatte : public Material
	{
	public:
		// Creates a matte material.
		// @param ka - The ambient factor.
		// @param kd - The diffuse factor.
		// @param pTex - A pointer to the texture to use.
		SVMatte(F32 ka, F32 kd, Texture2D * pTex);

		virtual ~SVMatte();

		virtual Color Shade(const ElementIntersection& ei, World& world)override;

	private:
		SVLambertian ambientBRDF;
		SVLambertian diffuseBRDF;
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

		virtual Color Shade(const ElementIntersection& ei, World& world)override;

	private:
		Lambertian ambientBRDF;
		Lambertian diffuseBRDF;
		GlossySpecular specularBRDF;
	};

	Material * LoadMaterial(const std::map<std::string, std::string>& params, const World* world);
	Material * LoadMaterial(const std::string& params, const World* world);
}