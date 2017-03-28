#include <Camera.h>

namespace RE
{
	Camera::Camera()
	{
		CalculateBasis(VML::VECTOR3F(0.0f, 0.0f, 0.0f), VML::VECTOR3F(0.0f, 0.0f, -1.0f), VML::VECTOR3F(0.0f, 1.0f, 0.0f));
	}

	Camera::Camera(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up)
	{
		CalculateBasis(eyePos, lookat, up);
	}

	Camera::~Camera()
	{

	}

	void Camera::CalculateBasis(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up)
	{
		eye = VML::Vector(eyePos);

		w = (eye - VML::Vector(lookat)).v3Normalize();
		u = (VML::Vector(up).v3Cross(w)).v3Normalize();
		v = w.v3Cross(u);
	}

	void Camera::SetViewport(Viewport vp)
	{
		this->vp = vp;
	}

	Viewport Camera::GetViewport()const
	{
		return vp;
	}

	void Camera::SetBackgroundColor(Color color)
	{
		this->backgroundColor = color;
	}


	VML::Vector Camera::GetPosition()const
	{
		return eye;
	}
}