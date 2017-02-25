#pragma once

#include <string>
#include <World.h>
#include <Camera.h>

namespace RE
{
	class RenderingEngine
	{
	public:
		RenderingEngine();

		// Default destructor.
		virtual ~RenderingEngine() = default;

		// Initializes the rendering engine and loads any necessary components.
		// @param configFile - The filename of the configuration file to read from.
		void Initialize(const std::string& configFile);

		// Get a reference to the world to draw.
		// @param pWorld - A pointer to the world.
		void SetWorld(World* pWorld);

		// Run the ray tracer on the world.
		void RunRayTracer();

		// Unload the rendering engine components.
		void Unload();

	public:
		static U32 NumSamples;
		static SamplerGenerator SamplerFunc;

	private:
		Camera *pCamera;
		Color bkgColor;
		Viewport vp;

		World* pWorld;

		std::string outputFolder;
		std::string outputFilename;
	};
}