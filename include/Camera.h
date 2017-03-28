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
		Viewport(U32 width, U32 height, SamplerGenerator samplerType, I32 numSamples, U32 numThreads = 1);

		// Get the center (in world coordinates) on the pixel at (row, col).
		// @param row - The row of the pixel.
		// @param col - The col of the pixel.
		// @return The 3D world coordinates of this pixel.
		VML::VECTOR3F GetPixelCenter(U32 row, U32 col)const;

		Sampler* GetSampler(U32 threadIdx = 0);

		VML::VECTOR3F GetUnitSquareSample(U32 row, U32 col, U32 threadIdx = 0);

		void ScalePixelWidth(F32 scale);

		// Get the width of this viewport.
		U32 GetWidth()const;
		// Get the height of this viewport.
		U32 GetHeight()const;

	private:
		U32 width, height;
		F32 pixelWidth;
		std::vector<Sampler> samplers;
	};


	class Camera
	{
	public:
		// Default constructor.
		Camera();
		// Creates a camera
		Camera(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up);

		virtual ~Camera();

		// Sets the viewport of this camera.
		virtual void SetViewport(Viewport vp);
		// Gets the viewport.
		Viewport GetViewport()const;

		// Sets the background color.
		void SetBackgroundColor(Color color);

		// Renders a single pixel.
		// @param row - The pixel's row.
		// @param col - The pixel's column.
		// @param pWorld - A pointer to the world to render.
		virtual Color RenderPixel(U32 row, U32 col, World* pWorld, U32 threadIdx = 0) = 0;

		// Gets the position of this camera.
		VML::Vector GetPosition()const;

	private:
		void CalculateBasis(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up);

	protected:
		VML::Vector eye;
		VML::Vector u, v, w;
		F32 maxDepth;

		Viewport vp;
		Color backgroundColor;

		friend class RenderingEngine;
	};

	
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera();

		// Creates an orthographic camera.
		// @param eyePos - The position of this camera.
		// @param lookat - The lookat point of the camera.
		// @param up - The world up vector.
		// @param halfWidth - The halfwidth of the viewport window.
		OrthographicCamera(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up,
							F32 halfWidth);

		// Renders a single pixel.
		// @param row - The pixel's row.
		// @param col - The pixel's column.
		// @param pWorld - A pointer to the world to render.
		virtual Color RenderPixel(U32 row, U32 col, World* pWorld, U32 threadIdx = 0);

	protected:
		VML::Vector forwardDirection;
		F32 halfWidth;
	};


	
	class PinholeCamera : public Camera
	{
	public:
		PinholeCamera();

		// Creates a pinhole camera.
		// @param eyePos - The position of this camera.
		// @param lookat - The lookat point of the camera.
		// @param up - The world up vector.
		// @param distanceToViewPlane - The distance from the camera to the view plane.
		// @param zoom - The zoom factor of this camera.
		PinholeCamera(const VML::VECTOR3F& eyePos, const VML::VECTOR3F& lookat, const VML::VECTOR3F& up,
			F32 distanceToViewPlane, F32 zoom);

		// Sets the viewport of this camera.
		virtual void SetViewport(Viewport vp) override;

		// Renders a single pixel.
		// @param row - The pixel's row.
		// @param col - The pixel's column.
		// @param pWorld - A pointer to the world to render.
		virtual Color RenderPixel(U32 row, U32 col, World* pWorld, U32 threadIdx = 0);

	protected:
		// Distance to the viewplane
		F32 d;

		F32 zoom;
	};
}