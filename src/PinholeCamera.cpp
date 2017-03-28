#include <Camera.h>

namespace RE
{
	PinholeCamera::PinholeCamera() : Camera()
	{

	}

	PinholeCamera::PinholeCamera(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up,
								F32 distanceToViewPlane, F32 zoom) : Camera(eyePos, lookat, up)
	{
		d = distanceToViewPlane;
		this->zoom = zoom;
	}


	void PinholeCamera::SetViewport(Viewport vp)
	{
		Camera::SetViewport(vp);

		this->vp.ScalePixelWidth(zoom);
	}

	Color PinholeCamera::RenderPixel(U32 row, U32 col, World* pWorld, U32 threadIdx)
	{
		VML::VECTOR3F pixelCoords = vp.GetUnitSquareSample(row, col, threadIdx);
		VML::Vector rayDir = ((u * pixelCoords.x) + (v * pixelCoords.y) - (w * d)).v3Normalize();
		Ray ray(eye, rayDir);

		RayTraceOutput rto;
		bool bHit = pWorld->TraceRay(rto, ray);
		rto.color.a = 1.0f;
		rto.color.Clamp();

		if (!bHit)
			rto.color = backgroundColor;

		return rto.color;
	}
}