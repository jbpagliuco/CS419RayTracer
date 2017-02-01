#pragma once

#include <Util.h>
#include <VML.h>

namespace RE
{
	typedef std::vector<VML::VECTOR2F>(*SamplerGenerator)(I32, I32);

	class Sampler
	{
	public:
		// Default constructor
		Sampler();

		Sampler(I32 numSamples);

		// Destructor
		virtual ~Sampler();

		I32 GetNumSamples()const;

		// Generates the samples according to the sampling generator function.
		// @param gen - The generator function.
		void GenerateSamples(SamplerGenerator gen);

		// Get the next sample on the unit square.
		VML::VECTOR2F SampleUnitSquare();

	private:
		// Sets up the shuffled sampling indices
		void SetupShuffledIndices();

	private:
		// Number of sample points in a set
		I32 numSamples;
		// Number of sample sets
		I32 numSets;

		// Sample points on a unit square
		std::vector<VML::VECTOR2F> samples;
		// Shuffled samples array indices
		std::vector<I32> shuffledIndices;

		// The current number of sample points used
		U32 count;
		// Random jump index
		I32 jump;
	};



	std::vector<VML::VECTOR2F> RegularSampler(I32 numSamples, I32 numSets);
	std::vector<VML::VECTOR2F> MultiJitteredSampler(I32 numSamples, I32 numSets);

	SamplerGenerator GetSamplerGeneratorFromString(const std::string& str);
}