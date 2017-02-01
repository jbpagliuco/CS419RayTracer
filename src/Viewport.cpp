#include <RenderingEngine.h>

#include <assert.h>

namespace RE
{
	Viewport::Viewport(U32 width, U32 height, SamplerGenerator samplerType, I32 numSamples)
	{
		this->width = width;
		this->height = height;
		this->pixelWidth = 2.0f / (F32)width;

		sampler = Sampler(numSamples);
		sampler.GenerateSamples(samplerType);
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

	VML::VECTOR3F Viewport::GetUnitSquareSample(U32 row, U32 col)
	{
		assert(row >= 0 && row < height && col >= 0 && col < width);

		VML::VECTOR2F sp = sampler.SampleUnitSquare();

		VML::VECTOR3F p;
		p.x = pixelWidth * (col - (width / 2.0f) + sp.x);
		p.y = -pixelWidth * (row - (height / 2.0f) + sp.y);
		p.z = 0.0f;

		return p;
	}


	Sampler * Viewport::GetSampler()
	{
		return &sampler;
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