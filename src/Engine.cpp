#include "Engine.h"

#include <Log.h>
#include <P4ConfigReader.h>

namespace RE
{
	Engine::Engine()
	{

	}

	void Engine::Initialize(const std::string& configFile)
	{
		RE_LOG(ENGINE, INIT, "Initializing engine");

		P4::ConfigReader::Reader c;
		
		std::string worldFile;
		std::string reConfig;

		c.addSetting("world", &worldFile, P4_CR_STRING);
		c.addSetting("re_config", &reConfig, P4_CR_STRING);

		c.importSettings(configFile);

		renderingEngine.Initialize(reConfig);

		world.LoadFromFile(worldFile);
		
	    renderingEngine.SetWorld(&world);
	}

	void Engine::Unload()
	{
		renderingEngine.Unload();

		world.DestroyWorld();
	}

	void Engine::Run()
	{
		renderingEngine.RunRayTracer();
	}
}