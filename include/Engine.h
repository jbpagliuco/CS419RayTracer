#pragma once

#include <string>
#include <RenderingEngine.h>

namespace RE
{
	class Engine
	{
	public:
		Engine();

		// Default destructor.
		~Engine() = default;

		// Initialize the engine and all subsystems.
		// @param configFile - The configuration file for the engine.
		void Initialize(const std::string& configFile);

		// Unload all subsystems.
		void Unload();

		// Run the rendering engine.
		void Run();

	private:
		World world;
		RenderingEngine renderingEngine;
	};
}