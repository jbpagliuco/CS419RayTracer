#include <Mesh.h>

#include <algorithm>

namespace RE
{
	MeshTriangle::MeshTriangle(Mesh * pMesh, const Face& face) : pMesh(pMesh), face(face)
	{

	}

	MeshTriangle::~MeshTriangle()
	{

	}

	bool MeshTriangle::Intersects(RayIntersectionList& outHitInfo, const Ray& ray, const Transform& elementTransform)const
	{
		VML::VECTOR3F v0 = (elementTransform.position + VML::Vector(pMesh->positions[face.v[0].vertex])).asVector3();
		VML::VECTOR3F v1 = (elementTransform.position + VML::Vector(pMesh->positions[face.v[1].vertex])).asVector3();
		VML::VECTOR3F v2 = (elementTransform.position + VML::Vector(pMesh->positions[face.v[2].vertex])).asVector3();

		D64 a = v0.x - v1.x, b = v0.x - v2.x, c = ray.GetDirection().getX(), d = v0.x - ray.GetOrigin().getX();
		D64 e = v0.y - v1.y, f = v0.y - v2.y, g = ray.GetDirection().getY(), h = v0.y - ray.GetOrigin().getY();
		D64 i = v0.z - v1.z, j = v0.z - v2.z, k = ray.GetDirection().getZ(), l = v0.z - ray.GetOrigin().getZ();

		D64 m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
		D64 q = g * i - e * k, s = e * j - f * i;

		D64 inv_denom = 1.0 / (a * m + b * q + c * s);

		D64 e1 = d * m - b * n - c * p;
		D64 beta = e1 * inv_denom;

		if (beta < 0.0)
			return false;

		D64 r = e * l - h * i;
		D64 e2 = a * n + d * q + c * r;
		D64 gamma = e2 * inv_denom;

		if (gamma < 0.0)
			return false;

		if (beta + gamma > 1.0)
			return false;

		D64 e3 = a * p - b * r + d * s;
		D64 t = e3 * inv_denom;

		if (t < VML::FLOAT_EPSILON)
			return false;

		outHitInfo.closestIntersection.t = (F32)t;

		outHitInfo.closestIntersection.normal =
			(VML::Vector(pMesh->normals[face.v[0].normal]) * (1.0f - (F32)beta - (F32)gamma)) +
			(VML::Vector(pMesh->normals[face.v[1].normal]) * (F32)beta) +
			(VML::Vector(pMesh->normals[face.v[2].normal]) * (F32)gamma);
		outHitInfo.closestIntersection.normal.v3Normalize();

		outHitInfo.closestIntersection.worldCoords = ray.GetPointAlongRay((F32)t);

		// We have texture coordinates
		if (face.v[0].bHasUV)
		{
			VML::VECTOR2F uv[3] = { pMesh->uvs[face.v[0].uv], pMesh->uvs[face.v[1].uv], pMesh->uvs[face.v[2].uv] };
			outHitInfo.closestIntersection.uv = VML::VECTOR2F(0.0f, 0.0f);

			outHitInfo.closestIntersection.uv.x += ((1.0f - (F32)beta - (F32)gamma) * uv[0].x);
			outHitInfo.closestIntersection.uv.y += ((1.0f - (F32)beta - (F32)gamma) * (1.0f - uv[0].y));

			outHitInfo.closestIntersection.uv.x += ((F32)beta * uv[1].x);
			outHitInfo.closestIntersection.uv.y += ((F32)beta * (1.0f - uv[1].y));

			outHitInfo.closestIntersection.uv.x += ((F32)gamma * uv[2].x);
			outHitInfo.closestIntersection.uv.y += ((F32)gamma * (1.0f - uv[2].y));
		}

		outHitInfo.intersections.push_back(outHitInfo.closestIntersection);
		outHitInfo.numIntersections = 1;

		return true;
	}

	bool MeshTriangle::Intersects(F32& tmin, const Ray& ray, const Transform& elementTransform)const
	{
		VML::VECTOR3F v0 = (elementTransform.position + VML::Vector(pMesh->positions[face.v[0].vertex])).asVector3();
		VML::VECTOR3F v1 = (elementTransform.position + VML::Vector(pMesh->positions[face.v[1].vertex])).asVector3();
		VML::VECTOR3F v2 = (elementTransform.position + VML::Vector(pMesh->positions[face.v[2].vertex])).asVector3();

		D64 a = v0.x - v1.x, b = v0.x - v2.x, c = ray.GetDirection().getX(), d = v0.x - ray.GetOrigin().getX();
		D64 e = v0.y - v1.y, f = v0.y - v2.y, g = ray.GetDirection().getY(), h = v0.y - ray.GetOrigin().getY();
		D64 i = v0.z - v1.z, j = v0.z - v2.z, k = ray.GetDirection().getZ(), l = v0.z - ray.GetOrigin().getZ();

		D64 m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
		D64 q = g * i - e * k, s = e * j - f * i;

		D64 inv_denom = 1.0 / (a * m + b * q + c * s);

		D64 e1 = d * m - b * n - c * p;
		D64 beta = e1 * inv_denom;

		if (beta < 0.0)
			return false;

		D64 r = e * l - h * i;
		D64 e2 = a * n + d * q + c * r;
		D64 gamma = e2 * inv_denom;

		if (gamma < 0.0)
			return false;

		if (beta + gamma > 1.0)
			return false;

		D64 e3 = a * p - b * r + d * s;
		D64 t = e3 * inv_denom;

		if (t < VML::FLOAT_EPSILON)
			return false;

		tmin = (F32)t;
		return true;
	}

	BoundingBox MeshTriangle::GetBoundingBox()const
	{
		VML::VECTOR3F v1 = pMesh->positions[face.v[0].vertex];
		VML::VECTOR3F v2 = pMesh->positions[face.v[1].vertex];
		VML::VECTOR3F v3 = pMesh->positions[face.v[2].vertex];

		VML::VECTOR3F min(
			std::min(v1.x, std::min(v2.x, v3.x)),
			std::min(v1.y, std::min(v2.y, v3.y)),
			std::min(v1.z, std::min(v2.z, v3.z)));

		VML::VECTOR3F max(
			std::max(v1.x, std::max(v2.x, v3.x)),
			std::max(v1.y, std::max(v2.y, v3.y)),
			std::max(v1.z, std::max(v2.z, v3.z)));

		return BoundingBox(min, max);
	}
}