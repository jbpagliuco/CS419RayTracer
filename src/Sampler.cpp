#include <Sampler.h>

#include <algorithm>

namespace RE
{
	Sampler::Sampler()
	{
		numSamples = 1;
		numSets = 83;
		count = 0;
		jump = 0;

		samples.reserve(numSamples * numSets);
		SetupShuffledIndices();
	}

	Sampler::Sampler(I32 numSamples)
	{
		this->numSamples = numSamples;
		numSets = 83;
		count = 0;
		jump = 0;

		samples.reserve(numSamples * numSets);
		SetupShuffledIndices();
	}

	Sampler::~Sampler()
	{

	}


	I32 Sampler::GetNumSamples()const
	{
		return numSamples;
	}
	
	void Sampler::SetupShuffledIndices()
	{
		shuffledIndices.reserve(numSamples * numSets);
		std::vector<I32> indices;

		for (int j = 0; j < numSamples; j++)
			indices.push_back(j);

		for (int p = 0; p < numSets; p++) {
			random_shuffle(indices.begin(), indices.end());

			for (int j = 0; j < numSamples; j++)
				shuffledIndices.push_back(indices[j]);
		}
	}

	void Sampler::GenerateSamples(SamplerGenerator gen)
	{
		samples = gen(numSamples, numSets);
	}

	VML::VECTOR2F Sampler::SampleUnitSquare()
	{
		if (count % numSamples == 0)  							// start of a new pixel
			jump = (rand() % numSets) * numSamples;				// random index jump initialised to zero in constructor

		return (samples[jump + shuffledIndices[jump + count++ % numSamples]]);
	}






	// -------------------------------------------------------------------------------- Regular Sample Generator
	std::vector<VML::VECTOR2F> RegularSampler(I32 numSamples, I32 numSets)
	{
		std::vector<VML::VECTOR2F> samples;

		I32 n = (I32)sqrt((F32)numSamples);

		for (I32 j = 0; j < numSets; j++)
			for (I32 p = 0; p < n; p++)
				for (I32 q = 0; q < n; q++)
					samples.push_back(VML::VECTOR2F((q + 0.5f) / n, (p + 0.5f) / n));

		return samples;
	}


	// -------------------------------------------------------------------------------- Multi-Jittered Sample Generator
	std::vector<VML::VECTOR2F> MultiJitteredSampler(I32 numSamples, I32 numSets)
	{
		// numSamples needs to be a perfect square
		std::vector<VML::VECTOR2F> samples;
		samples.reserve(numSamples * numSets);
			
		I32 n = (I32)sqrt((F32)numSamples);
		F32 subcell_width = 1.0f / ((F32)numSamples);

		// fill the samples array with dummy points to allow us to use the [ ] notation when we set the 
		// initial patterns

		VML::VECTOR2F fill_point;
		for (I32 j = 0; j < numSamples * numSets; j++)
			samples.push_back(fill_point);

		// distribute points in the initial patterns

		for (I32 p = 0; p < numSets; p++)
			for (I32 i = 0; i < n; i++)
				for (I32 j = 0; j < n; j++) {
					samples[i * n + j + p * numSamples].x = (i * n + j) * subcell_width + RandFloat(0, subcell_width);
					samples[i * n + j + p * numSamples].y = (j * n + i) * subcell_width + RandFloat(0, subcell_width);
				}

		// shuffle x coordinates

		for (I32 p = 0; p < numSets; p++)
			for (I32 i = 0; i < n; i++)
				for (I32 j = 0; j < n; j++) {
					I32 k = RandInt(j, n - 1);
					F32 t = samples[i * n + j + p * numSamples].x;
					samples[i * n + j + p * numSamples].x = samples[i * n + k + p * numSamples].x;
					samples[i * n + k + p * numSamples].x = t;
				}

		// shuffle y coordinates

		for (I32 p = 0; p < numSets; p++)
			for (I32 i = 0; i < n; i++)
				for (I32 j = 0; j < n; j++) {
					I32 k = RandInt(j, n - 1);
					F32 t = samples[j * n + i + p * numSamples].y;
					samples[j * n + i + p * numSamples].y = samples[k * n + i + p * numSamples].y;
					samples[k * n + i + p * numSamples].y = t;
				}

		return samples;
	}





	SamplerGenerator GetSamplerGeneratorFromString(const std::string& str)
	{
		if (str == "multi_jittered")
			return MultiJitteredSampler;

		return RegularSampler;
	}
}