#include "Geometry.h"

namespace RE
{
	Ray::Ray()
	{
		this->origin = VML::VectorZero();
		this->direction = VML::Vector(0.0f, 0.0f, 1.0f, 0.0f);
		this->invD = (VML::Vector(1.0f) / this->direction).asVector3();
	}

	Ray::Ray(const VML::VECTOR3F& origin, const VML::VECTOR3F& direction)
	{
		this->origin = VML::Vector(origin);
		this->direction = VML::Vector(direction.x, direction.y, direction.z, 0.0f).v3Normalize();
		this->invD = (VML::Vector(1.0f) / this->direction).asVector3();
	}

	Ray::Ray(const VML::Vector& origin, const VML::Vector& direction, F32 epsilon)
	{
		this->origin = VML::Vector(origin);
		this->direction = VML::Vector(direction).v3Normalize();
		this->invD = (VML::Vector(1.0f) / this->direction).asVector3();

		this->origin += this->direction * epsilon;
	}

	VML::Vector Ray::GetPointAlongRay(F32 t)const
	{
		VML::Vector v = origin + (direction * t);
		return v;
	}


	VML::Vector Ray::GetOrigin()const
	{
		return origin;
	}

	VML::Vector Ray::GetDirection()const
	{
		return direction;
	}

	VML::VECTOR3F Ray::GetInvDirection()const
	{
		return invD;
	}
}