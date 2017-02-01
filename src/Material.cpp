#include <Material.h>

#include <sstream>

namespace RE
{
	Material::Material()
	{

	}

	Material::Material(F32 ambient, Color diffuse, Color specular, F32 shininess)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
	}

	Material::~Material()
	{

	}


	F32 Material::GetAmbient()const
	{
		return ambient;
	}

	Color Material::GetDiffuseColor()const
	{
		return diffuse;
	}

	Color Material::GetSpecularColor()const
	{
		return specular;
	}

	F32 Material::GetShininess()const
	{
		return shininess;
	}





	Material * CreateMaterialFromString(const std::string& desc)
	{
		std::istringstream iss(desc);

		std::string garbage;
		iss >> garbage;

		F32 ambient;
		Color diffuse, specular;
		F32 shininess;

		iss >> ambient;
		iss >> diffuse.r >> diffuse.g >> diffuse.b >> diffuse.a;
		iss >> specular.r >> specular.g >> specular.b >> specular.a;
		iss >> shininess;

		return new Material(ambient, diffuse, specular, shininess);
	}
}