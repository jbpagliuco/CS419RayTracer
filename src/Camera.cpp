#include <Camera.h>

#include <Log.h>

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

		// Create render target buffer and depth buffer
		renderBuffer = ColorBuffer2D(vp.GetWidth(), vp.GetHeight(), true);
		depthBuffer = Buffer2D<F32>(vp.GetWidth(), vp.GetHeight(), false);

		depthBuffer.Fill(maxDepth);

		imageTracker.SetBuffer(&renderBuffer);
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




	const ColorBuffer2D& Camera::RenderScene(World* pWorld)
	{
		U32 pixelIndex = 0;
		U32 totalNumPixels = vp.GetWidth() * vp.GetHeight();
		I32 percent = -1;

		U32 width = vp.GetWidth();
		U32 height = vp.GetHeight();
		U32 numSamples = vp.GetSampler()->GetNumSamples();

		for (U32 row = 0; row < height; row++)
		{
			for (U32 col = 0; col < width; col++)
			{
				F32 newPercent = ((F32)pixelIndex / (F32)totalNumPixels) * 100.0f;
				if (newPercent >= percent + 1)
				{
					RE_LOG_SAMELINE(RENDER_ENG, RUNTIME, "Progress: " << ++percent << "%");
					imageTracker.Update(row, col);
				}
				pixelIndex++;

				Color finalColor(0.0f, 0.0f, 0.0f, 1.0f);
				for (U32 s = 0; s < numSamples; s++)
				{
					Color c = RenderPixel(row, col, pWorld);

					finalColor = finalColor + c;
				}

				finalColor = finalColor / (F32)vp.GetSampler()->GetNumSamples();
				finalColor.Clamp();

				renderBuffer(row, col) = finalColor;
			}
		}

		RE_LOG_NEWLINE();

		imageTracker.Update(vp.GetHeight() - 1, vp.GetWidth() - 1);

		return renderBuffer;
	}
}