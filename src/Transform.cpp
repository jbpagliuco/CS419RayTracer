#include <Transform.h>

#include <sstream>

namespace RE
{
	Transform::Transform() : position(VML::VectorZero())
	{

	}

	Transform::Transform(const VML::VECTOR3F position) : position(position)
	{

	}

	Transform::Transform(const VML::Vector& position) : position(position)
	{

	}

	Transform::~Transform()
	{

	}




	Transform CreateTransformFromString(const std::string& desc)
	{
		std::string garbage;
		VML::VECTOR3F position;

		std::stringstream ss(desc);
		ss >> garbage;

		ss >> position.x >> position.y >> position.z;

		return Transform(position);
	}
}