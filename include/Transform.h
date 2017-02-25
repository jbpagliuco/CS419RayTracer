#pragma once

#include <Util.h>
#include <VML.h>
#include <map>

namespace RE
{
	// The positional/rotation representation of a WorldElement.
	RE_ALIGN_MS(16) class Transform
	{
	public:
		Transform();
		// Creates a transform with the given position.
		// @param position - The position, in world coordinates.
		Transform(const VML::VECTOR3F position);
		// Creates a transform with the given position.
		// @param position - The position, in world coordinates.
		Transform(const VML::Vector& position);

		virtual ~Transform();

	public:
		VML::Vector position;
	} RE_ALIGN_GCC(16);


	Transform LoadTransform(const std::string& params);
}