#include <KDTree.h>

namespace RE
{
	BoundingBox KDType::GetTransformedBBox()const
	{
		return GetBoundingBox().TransformBox(GetTransform());
	}
}