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

		F32 GetAmbient()const;
		Color GetDiffuseColor()const;
		Color GetSpecularColor()const;
		F32 GetShininess()const;

	protected:
		F32 ambient;
		Color diffuse;
		Color specular;
		F32 shininess;
	};

	Material * CreateMaterialFromString(const std::string& desc);
}