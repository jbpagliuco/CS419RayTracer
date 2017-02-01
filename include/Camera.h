#pragma once

#include <VML.h>
#include <World.h>
#include <Buffer.h>
#include <Sampler.h>
#include <ImageCV.h>

namespace RE
{
	class Viewport
	{
	public:
		// Default constructor.
		Viewport() = default;
		// Creates a viewport with the specified width and height.
		// @param width - The width of the viewport.
		// @param height - The height of the viewport.
		// @param samplerType - The type of sampler to use.
		// @param numSamples - The number of samples to use.
		Viewport(U32 width, U32 height, SamplerGenerator samplerType, I32 numSamples);

		// Get the center (in world coordinates) on the pixel at (row, col).
		// @param row - The row of the pixel.
		// @param col - The col of the pixel.
		// @return The 3D world coordinates of this pixel.
		VML::VECTOR3F GetPixelCenter(U32 row, U32 col)const;

		Sampler* GetSampler();

		VML::VECTOR3F GetUnitSquareSample(U32 row, U32 col);

		void ScalePixelWidth(F32 scale);

		// Get the width of this viewport.
		U32 GetWidth()const;
		// Get the height of this viewport.
		U32 GetHeight()const;

	private:
		U32 width, height;
		F32 pixelWidth;

		Sampler sampler;
	};


	class Camera
	{
	public:
		// Default constructor.
		Camera();
		// Creates a camera
		Camera(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up);

		virtual ~Camera();

		virtual void SetViewport(Viewport vp);
		Viewport GetViewport()const;

		void SetBackgroundColor(Color color);

		// Renders the scene from the viewpoint of this camera.
		// @param pWorld - A pointer to the world to render.
		// @return The resulting color buffer.
		virtual const ColorBuffer2D& RenderScene(World* pWorld);

		virtual Color RenderPixel(U32 row, U32 col, World* pWorld) = 0;

		VML::Vector GetPosition()const;

	private:
		void CalculateBasis(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up);

	protected:
		VML::Vector eye;
		VML::Vector u, v, w;
		F32 maxDepth;

		Viewport vp;
		Color backgroundColor;

		ColorBuffer2D renderBuffer;
		Buffer2D<F32> depthBuffer;
		ImageTracker imageTracker;
	};

	
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera();

		OrthographicCamera(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up,
							F32 halfWidth);

		virtual Color RenderPixel(U32 row, U32 col, World* pWorld);

	protected:
		VML::Vector forwardDirection;
		F32 halfWidth;
	};


	
	class PinholeCamera : public Camera
	{
	public:
		PinholeCamera();

		PinholeCamera(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up,
			F32 distanceToViewPlane, F32 zoom);

		virtual void SetViewport(Viewport vp) override;

		virtual Color RenderPixel(U32 row, U32 col, World* pWorld);

	protected:
		// Distance to the viewplane
		F32 d;

		F32 zoom;
	};
}