#include <RenderingEngine.h>

#include <Log.h>

namespace RE
{
	void RenderingEngine::RenderScene_Single()
	{
		U32 width = vp.GetWidth();
		U32 height = vp.GetHeight();
		U32 numSamples = vp.GetSampler()->GetNumSamples();

		U32 pixelIndex = 0;
		U32 totalNumPixels = vp.GetWidth() * vp.GetHeight();
		U32 updateFreq = 800 / numSamples;

		auto start = GetTime();

		for (U32 row = 0; row < height; row++)
		{
			for (U32 col = 0; col < width; col++)
			{
				if (pixelIndex % updateFreq == 0)
				{
					imageTracker.Update(row, col);
					F32 percent = (F32)pixelIndex / (F32)totalNumPixels * 100.0f;

					auto elapsed = DiffTime(start, GetTime());
					D64 total = 100.0 * elapsed / percent;

					RE_LOG_SAMELINE(RENDER_ENG, RUNTIME, "Progress: " << percent << "% ("
						<< "E: " << (elapsed / 1000000. / 60.)
						<< ", R: " << ((total - elapsed) / 1000000. / 60.) << ")");

				}

				pixelIndex++;

				Color finalColor(0.0f, 0.0f, 0.0f, 1.0f);
				for (U32 s = 0; s < numSamples; s++)
				{
					Color c = pCamera->RenderPixel(row, col, pWorld);

					finalColor = finalColor + c;
				}

				finalColor = finalColor / (F32)vp.GetSampler()->GetNumSamples();
				finalColor.Clamp();

				renderBuffer(row, col) = finalColor;
			}
		}

		RE_LOG_NEWLINE();

		imageTracker.Update(vp.GetHeight() - 1, vp.GetWidth() - 1);
	}
}