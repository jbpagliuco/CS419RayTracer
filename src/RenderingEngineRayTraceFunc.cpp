#include <RenderingEngine.h>

#include <Log.h>
#include <fstream>
#include <direct.h>

namespace RE
{
	// RIGHT HANDED COORDINATE SYSTEM
	void RenderingEngine::RunRayTracer()
	{
		if (!pWorld)
		{
			RE_LOGERROR(RENDER_ENG, RUNTIME, "World reference not set. Not able to draw scene.");
			return;
		}

		RE_LOG(RENDER_ENG, RUNTIME, "Running ray tracing algorithm...");

		// Start time
		auto start = GetTime();

		// Run ray tracer
		const ColorBuffer2D& result = pCamera->RenderScene(pWorld);
		
		// Total running time for the ray tracer
		U64 elapsedTime = DiffTime(start, GetTime());
		D64 elapsedSeconds = ((D64)elapsedTime / 1000000.0);
		D64 elapsedMinutes = elapsedSeconds / 60.0;

		// Viewport stats
		U32 width = pCamera->GetViewport().GetWidth();
		U32 height = pCamera->GetViewport().GetHeight();
		U32 numSamples = pCamera->GetViewport().GetSampler()->GetNumSamples();

		// Other stats
		U64 numWorldElements = pWorld->GetWorldElements().size();
		U64 numPrimaryRays = numSamples * width * height;
		U32 numLights = pWorld->GetLights().size();

		std::stringstream ss;
		ss <<
			"------------------------------------------------------------" <<
			"\nElapsed Time: " << elapsedTime << " usec (" << elapsedSeconds << " sec) (" << elapsedMinutes << " min)" << 
			"\nNumber of objects in world: " << numWorldElements <<
			"\nNumber of lights in world: " << numLights <<
			"\nNumber of samples per pixel: " << numSamples <<
			"\nTotal number of rays shot: " << numPrimaryRays <<
			"\nRender target stats: " <<
			"\n\tResolution: " << width << "x" << height <<
			"\n\tTotal number of pixels rendered: " << width * height <<
			"\n\tAverage time per pixel: " << (D64)elapsedTime / ((D64)width * (D64)height) << " usec" <<
			"\n------------------------------------------------------------";

		// Dump stats to console
		std::string stats = ss.str();
		RE_LOG(RENDER_ENG, RUNTIME, "RENDER STATS\n" << stats);
		

		// Dump results to file
		_mkdir(outputFolder.c_str());

		std::stringstream outputFileSS;
		outputFileSS << outputFolder << "/result-" <<
			width << "x" << height << "-" <<
			numSamples << "SAMPLES" <<
			".png";
		RE_LOG(RENDER_ENG, RUNTIME, "Writing result image to " << outputFileSS.str());

		result.SaveToImage(outputFileSS.str());

		outputFileSS << ".stats";
		RE_LOG(RENDER_ENG, RUNTIME, "Writing result stats to " << outputFileSS.str());
		std::ofstream out(outputFileSS.str());
		out << ss.str();
		out.close();
	}
}