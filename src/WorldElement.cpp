#include <World.h>

#include <P4Mem.h>

namespace RE
{
	WorldElement::WorldElement(const std::string& name)
	{
		this->name = name;
	}

	WorldElement::WorldElement(const WorldElement& other)
	{
		Copy(other);
	}

	WorldElement::WorldElement(WorldElement&& other)
	{
		pGeometry = other.pGeometry;
		pMaterial = other.pMaterial;

		other.pGeometry = nullptr;
		other.pMaterial = nullptr;
	}

	WorldElement::~WorldElement()
	{
		Destroy();
	}

	WorldElement& WorldElement::operator=(const WorldElement& other)
	{
		if (this != &other)
		{
			Destroy();
			Copy(other);
		}

		return *this;
	}

	WorldElement& WorldElement::operator=(WorldElement&& other)
	{
		if (this != &other)
		{
			Destroy();

			pGeometry = other.pGeometry;
			pMaterial = other.pMaterial;

			other.pGeometry = nullptr;
			other.pMaterial = nullptr;
		}

		return *this;
	}

	void WorldElement::Copy(const WorldElement& other)
	{
		// Uh
		pGeometry = other.pGeometry;
		pMaterial = other.pMaterial;
	}

	void WorldElement::Destroy()
	{
		if (pMaterial)
		{
			delete pMaterial;
			pMaterial = nullptr;
		}
	}
}