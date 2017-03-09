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
		triangles.push_back(MeshTriangle(this, face));
	}

	void Mesh::Build()
	{
		grid.AddObjects(triangles);
	}

	bool Mesh::Intersects(RayIntersectionList& outHitInfo, const Ray& ray, const Transform& elementTransform)const
	{
		Ray transformedRay(ray.GetOrigin() - elementTransform.position, ray.GetDirection());

		MeshTriangleIntersection gi = grid.Traverse(transformedRay);
		
		outHitInfo = gi.rl;
		return gi.bHit;
	}

	bool Mesh::Intersects(F32& t, const Ray& ray, const Transform& elementTransform)const
	{
		Ray transformedRay(ray.GetOrigin() - elementTransform.position, ray.GetDirection());

		return grid.TraverseShallow(transformedRay, F32_MAX);
	}

	BoundingBox Mesh::GetBoundingBox()const
	{
		return grid.GetBounds();
	}
}