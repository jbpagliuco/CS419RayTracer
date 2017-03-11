#include <Mesh.h>

namespace RE
{
	KDTypeMesh::KDTypeMesh(MeshTriangle t) : t(t)
	{

	}

	KDTypeMesh::~KDTypeMesh()
	{

	}

	Geometry * KDTypeMesh::GetGeometry()const
	{
		return (Geometry*)&t;
	}

	Transform KDTypeMesh::GetTransform()const
	{
		return Transform();
	}

	BoundingBox KDTypeMesh::GetBoundingBox()const
	{
		return t.GetBoundingBox();
	}
}