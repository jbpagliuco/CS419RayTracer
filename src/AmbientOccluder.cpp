#include <Light.h>

#include <World.h>

namespace RE
{
	AmbientOccluder::AmbientOccluder()
	{

	}

	AmbientOccluder::AmbientOccluder(F32 ls, Color color, SamplerGenerator gen, U32 numSamples, F32 minAmount) : 
		ls(ls), color(color), minAmount(minAmount)
	{
		sampler = Sampler(numSamples);
		sampler.GenerateSamples(gen);
		sampler.MapSamplesToHemisphere(1.0f);
	}

	AmbientOccluder::~AmbientOccluder()
	{

	}

	F32 AmbientOccluder::GetDistanceFromPoint(const VML::Vector& point)const
	{
		return 0.5f;
	}

	VML::Vector AmbientOccluder::GetDirectionFromPoint(const VML::Vector& point)const
	{
		return VML::VectorZero();
	}
	
	VML::Vector AmbientOccluder::SampleDirection()
	{
		VML::VECTOR3F sp = sampler.SampleHemisphere();
		return u * sp.x + v * sp.y + w * sp.z;
	}

	Color AmbientOccluder::CalculateRadiance(const ElementIntersection& ei, const World& world)
	{
		w = ei.rayInt.normal;
		v = w.v3Cross(VML::Vector(0.0072f, 1.0f, 0.0034f, 0.0f));
		v.v3Normalize();
		u = v.v3Cross(w);

		Ray shadowRay(ei.rayInt.worldCoords, SampleDirection(), RAY_EPSILON);
		if (InShadow(shadowRay, ei, world))
			return color * ls * minAmount;
		else
			return color * ls;
	}
}