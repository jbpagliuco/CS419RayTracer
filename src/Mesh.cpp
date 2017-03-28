#include <Mesh.h>

namespace RE
{
	Mesh::Mesh()
	{

	}

	Mesh::~Mesh()
	{

	}

	void Mesh::AddVertexPosition(const VML::VECTOR3F& pos)
	{
		positions.push_back(pos);
	}

	void Mesh::AddVertexNormal(const VML::VECTOR3F& normal)
	{
		normals.push_back(normal);
	}

	void Mesh::AddVertexUV(const VML::VECTOR2F& uv)
	{
		uvs.push_back(uv);
	}

	void Mesh::ComputeNormals()
	{
		
	}

	void Mesh::AddFace(const Face& face)
	{
		MeshTriangle t(this, face);
		KDTypeMesh kd(t);
		triangles.push_back(kd);
	}

	void Mesh::Build()
	{
		tree = KDTree<KDTypeMesh>(triangles);
	}

	bool Mesh::Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const
	{
		KDTypeMesh hit;
		return tree.Traverse(outHitInfo, ray, hit);
	}

	bool Mesh::Intersects(F32& t, const Ray& ray)const
	{
		return tree.TraverseShallow(ray, F32_MAX);
	}

	BoundingBox Mesh::GetBoundingBox()const
	{
		return tree.GetBoundingBox();
	}
}