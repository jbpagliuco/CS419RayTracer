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

		config.addSetting("use_multithreading", &bUseMultiThreading, P4_CR_BOOL);
		config.addSetting("num_threads", &numThreads, P4_CR_U8);

		config.importSettings(configFile);

		if (!bUseMultiThreading)
			numThreads = 1;

		SamplerFunc = GetSamplerGeneratorFromString(vpSamplerType);
		NumSamples = numSamples;

		// Create viewport
		if (numSamples == 1)
			vp = Viewport(resX, resY, RegularSampler, numSamples, numThreads);
		else
			vp = Viewport(resX, resY, SamplerFunc, numSamples, numThreads);

		// Create render target buffer and depth buffer
		renderBuffer = ColorBuffer2D(vp.GetWidth(), vp.GetHeight(), true);
		depthBuffer = Buffer2D<F32>(vp.GetWidth(), vp.GetHeight(), false);

		depthBuffer.Fill(F32_MAX);

		imageTracker.SetBuffer(&renderBuffer);
	}


	void RenderingEngine::SetWorld(World* pWorld)
	{
		this->pWorld = pWorld;

		pCamera = pWorld->GetCamera();
		pCamera->SetViewport(vp);
		pCamera->SetBackgroundColor(bkgColor);
	}

	void RenderingEngine::RenderScene()
	{
		if (bUseMultiThreading)
			RenderScene_Multi();
		else
			RenderScene_Single();
	}


	void RenderingEngine::Unload()
	{
		RE_LOG(RENDER_ENG, END, "Unloading rendering engine...");
	}
}