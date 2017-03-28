#include <Camera.h>

#include <Log.h>

namespace RE
{
	OrthographicCamera::OrthographicCamera() : Camera()
	{

	}

	OrthographicCamera::OrthographicCamera(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up,
											F32 halfWidth) 
										: Camera(eyePos, lookat, up)
	{
		this->halfWidth = halfWidth;

		forwardDirection = w;
		forwardDirection.negate();
	}






	Color OrthographicCamera::RenderPixel(U32 row, U32 col, World* pWorld, U32 threadIdx)
	{
		VML::VECTOR3F pixelCoords = vp.GetUnitSquareSample(row, col, threadIdx);
		pixelCoords.x *= halfWidth;
		pixelCoords.y *= halfWidth;
		VML::Vector rayOriginWorld = (u * pixelCoords.x) + (v * pixelCoords.y);
		Ray ray(eye + rayOriginWorld, forwardDirection);

		RayTraceOutput rto;
		bool bHit = pWorld->TraceRay(rto, ray);

		if (!bHit)
			rto.color = backgroundColor;
		rto.color.a = 1.0f;

		return rto.color;
	}
}