#include <RenderingEngine.h>

#include <assert.h>

namespace RE
{
	Viewport::Viewport(U32 width, U32 height, SamplerGenerator samplerType, I32 numSamples, U32 numThreads)
	{
		this->width = width;
		this->height = height;
		this->pixelWidth = 2.0f / (F32)width;

		for (I32 i = 0; i < (I32)numThreads; i++)
		{
			samplers.push_back(Sampler(numSamples));
			samplers[i].GenerateSamples(samplerType);
		}
	}

	VML::VECTOR3F Viewport::GetPixelCenter(U32 row, U32 col)const
	{
		assert(row >= 0 && row < width && col >= 0 && col < height);

		VML::VECTOR3F p;
		p.x = pixelWidth * (col - (width / 2.0f) + 0.5f);
		p.y = -pixelWidth * (row - (height / 2.0f) + 0.5f);
		p.z = 0.0f;

		return p;
	}

	VML::VECTOR3F Viewport::GetUnitSquareSample(U32 row, U32 col, U32 threadIdx)
	{
		assert(row >= 0 && row < height && col >= 0 && col < width);

		VML::VECTOR2F sp = samplers[threadIdx].SampleUnitSquare();

		VML::VECTOR3F p;
		p.x = pixelWidth * (col - (width / 2.0f) + sp.x);
		p.y = -pixelWidth * (row - (height / 2.0f) + sp.y);
		p.z = 0.0f;

		return p;
	}


	Sampler * Viewport::GetSampler(U32 threadIdx)
	{
		return &(samplers[threadIdx]);
	}


	U32 Viewport::GetWidth()const
	{
		return width;
	}

	U32 Viewport::GetHeight()const
	{
		return height;
	}


	void Viewport::ScalePixelWidth(F32 scale)
	{
		pixelWidth /= scale;
	}
}