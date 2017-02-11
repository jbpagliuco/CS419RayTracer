#pragma once

#include <Util.h>

namespace RE
{
	class Material
	{
	public:
		Material();
		Material(F32 ambient, Color diffuse, Color specular, F32 shininess);

		virtual ~Material();

		// Gets the ambient factor.
		F32 GetAmbient()const;
		// Gets the diffuse color.
		Color GetDiffuseColor()const;
		// Gets the specular color.
		Color GetSpecularColor()const;
		// Gets the shininess.
		F32 GetShininess()const;

	protected:
		F32 ambient;
		Color diffuse;
		Color specular;
		F32 shininess;
	};

	Material * CreateMaterialFromString(const std::string& desc);
}