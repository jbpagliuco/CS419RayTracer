#include "RenderingEngine.h"

#include <Log.h>
#include <P4ConfigReader.h>

namespace RE
{
	U32 RenderingEngine::NumSamples = 0;
	SamplerGenerator RenderingEngine::SamplerFunc = RegularSampler;

	RenderingEngine::RenderingEngine()
	{
		pWorld = nullptr;
	}


	void RenderingEngine::Initialize(const std::string& configFile)
	{
		RE_LOG(RENDER_ENG, INIT, "Initializing rendering engine");
		P4::ConfigReader::Reader config;

		U32 resX, resY;
		config.addSetting("res_x", &resX, P4_CR_U32);
		config.addSetting("res_y", &resY, P4_CR_U32);
		config.addSetting("bkg_color", &bkgColor, LoadColorFromConfigFile);
		config.addSetting("output_folder", &outputFolder, P4_CR_STRING);
		config.addSetting("output_name", &outputFilename, P4_CR_STRING);

		I32 numSamples;
		std::string vpSamplerType;
		config.addSetting("num_samples", &numSamples, P4_CR_I32);
		config.addSetting("viewport_sampler_type", &vpSamplerType, P4_CR_STRING);

		config.importSettings(configFile);

		SamplerFunc = GetSamplerGeneratorFromString(vpSamplerType);
		NumSamples = numSamples;

		// Create viewport
		if (numSamples == 1)
			vp = Viewport(resX, resY, RegularSampler, numSamples);
		else
			vp = Viewport(resX, resY, SamplerFunc, numSamples);
	}


	void RenderingEngine::SetWorld(World* pWorld)
	{
		this->pWorld = pWorld;

		pCamera = pWorld->GetCamera();
		pCamera->SetViewport(vp);
		pCamera->SetBackgroundColor(bkgColor);
	}


	void RenderingEngine::Unload()
	{
		RE_LOG(RENDER_ENG, END, "Unloading rendering engine...");
	}
}