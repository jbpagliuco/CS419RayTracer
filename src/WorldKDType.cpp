#include <World.h>

namespace RE
{
	KDTypeWorldElement::KDTypeWorldElement(WorldElement e) : e(e)
	{

	}

	KDTypeWorldElement::~KDTypeWorldElement()
	{

	}

	Geometry * KDTypeWorldElement::GetGeometry()const
	{
		return e.pGeometry;
	}

	Transform KDTypeWorldElement::GetTransform()const
	{
		return e.transform;
	}

	BoundingBox KDTypeWorldElement::GetBoundingBox()const
	{
		return e.pGeometry->GetBoundingBox().TransformBox(e.transform);
	}
}