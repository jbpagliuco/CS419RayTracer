#include <Geometry.h>

namespace RE
{
	Triangle::Triangle()
	{
		p1 = VML::VECTOR3F(-1.0f, -1.0f, 0.0f);
		p2 = VML::VECTOR3F(1.0f, -1.0f, 0.0f);
		p3 = VML::VECTOR3F(0.0f, 1.0f, 0.0f);

		VML::Vector a(p1);
		VML::Vector b(p2);
		VML::Vector c(p3);

		normal = ((b - a).v3Cross((c - a))).v3Normalize();
	}

	Triangle::Triangle(VML::VECTOR3F p1, VML::VECTOR3F p2, VML::VECTOR3F p3)
	{
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;

		VML::Vector a(p1);
		VML::Vector b(p2);
		VML::Vector c(p3);

		normal = ((b - a).v3Cross((c - a))).v3Normalize();
	}

	bool Triangle::Intersects(RayIntersectionList& outHitInfo, const Ray& ray, const Transform& elementTransform)const
	{
		VML::VECTOR3F v1 = (elementTransform.position + VML::Vector(p1)).asVector3();
		VML::VECTOR3F v2 = (elementTransform.position + VML::Vector(p2)).asVector3();
		VML::VECTOR3F v3 = (elementTransform.position + VML::Vector(p3)).asVector3();

		F32 a = v1.x - v2.x, b = v1.x - v3.x, c = ray.GetDirection().getX(), d = v1.x - ray.GetOrigin().getX();
		F32 e = v1.y - v2.y, f = v1.y - v3.y, g = ray.GetDirection().getY(), h = v1.y - ray.GetOrigin().getY();
		F32 i = v1.z - v2.z, j = v1.z - v3.z, k = ray.GetDirection().getZ(), l = v1.z - ray.GetOrigin().getZ();

		F32 m = f * k - g * i;
		F32 n = h * k - g * l;
		F32 p = f * l - h * j;

		F32 q = g * i - e * k;
		F32 s = e * j - f * i;

		F32 invDenom = 1.0f / (a * m + b * q + c * s);

		F32 e1 = d * m - b * n - c * p;
		F32 beta = e1 * invDenom;

		if (beta < 0.0f)
			return false;

		F32 r = e * l - h * i;
		F32 e2 = a * n + d * q + c * r;
		F32 gamma = e2 * invDenom;

		if (gamma < 0.0f)
			return false;

		if (beta + gamma > 1.0f)
			return false;

		F32 e3 = a * p - b * r + d * s;
		F32 t = e3 * invDenom;

		if (t < VML::FLOAT_EPSILON)
			return false;

		outHitInfo.closestIntersection.t = t;
		outHitInfo.closestIntersection.normal = normal;
		outHitInfo.closestIntersection.worldCoords = ray.GetPointAlongRay(t);
		outHitInfo.intersections.push_back(outHitInfo.closestIntersection);
		outHitInfo.numIntersections = 1;

		return true;
	}
}