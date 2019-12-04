/*! ****************************************************************************
\file             main.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  OCAE

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cassert>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <functional>
#include <thread>
#include <chrono>
#include <type_traits>

#include <RIFF-Util/RIFF.hpp>

#include "../Engine/Engine.hpp"

using hrc = std::chrono::high_resolution_clock;
using namespace OCAE;
using VoidFn = void(*)(void);

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

#define Equals(a,b) bool(double(std::abs(a-b) < EPSILON_F))

// Private Functions                      //////////////////////////////////////

static void TestResampler(void)
{
	std::vector<StereoData> samples{
		StereoData(SampleType(0), SampleType(0)),
		StereoData(SampleType(1), SampleType(1)),
		StereoData(SampleType(2), SampleType(2)),
		StereoData(SampleType(3), SampleType(3))
	};

	// Test increase of rate
	{
		Tools::Resampler resam(samples, SAMPLE_RATE/2);

		StereoData results[7];

		std::generate(
			results, results + (sizeof(results)/sizeof(*results)),
			[& resam]()->StereoData{
				return resam.SendSample();
			}
		);

		assert(Equals( Left(results[0]), SampleType(0)) &&
			   Equals(Right(results[0]), SampleType(0)));
		assert(Equals( Left(results[1]), SampleType(0.5)) &&
			   Equals(Right(results[1]), SampleType(0.5)));
		assert(Equals( Left(results[2]), SampleType(1)) &&
			   Equals(Right(results[2]), SampleType(1)));
		assert(Equals( Left(results[3]), SampleType(1.5)) &&
			   Equals(Right(results[3]), SampleType(1.5)));
		assert(Equals( Left(results[4]), SampleType(2)) &&
			   Equals(Right(results[4]), SampleType(2)));
		assert(Equals( Left(results[5]), SampleType(2.5)) &&
			   Equals(Right(results[5]), SampleType(2.5)));
		assert(Equals( Left(results[6]), SampleType(3)) &&
			   Equals(Right(results[6]), SampleType(3)));
	}

	// Test decrease of rate
	{
		Tools::Resampler resam(samples, SAMPLE_RATE * 2);

		StereoData results[2];

		std::generate(
			results, results + (sizeof(results)/sizeof(*results)),
			[& resam]()->StereoData{
				return resam.SendSample();
			}
		);

		assert(Equals( Left(results[0]), SampleType(0)) &&
			   Equals(Right(results[0]), SampleType(0)));
		assert(Equals( Left(results[1]), SampleType(2)) &&
			   Equals(Right(results[1]), SampleType(2)));
	}

	// Test playback speed
	{
		Tools::Resampler resam(samples, SAMPLE_RATE);

		StereoData results[8];

		resam.SetPlaybackSpeed(0.5);

		std::generate(
			results, results + (sizeof(results)/sizeof(*results)),
			[& resam]()->StereoData{
				return resam.SendSample();
			}
		);

		assert(Equals( Left(results[0]), SampleType(0)) &&
			   Equals(Right(results[0]), SampleType(0)));
		assert(Equals( Left(results[1]), SampleType(0.5)) &&
			   Equals(Right(results[1]), SampleType(0.5)));
		assert(Equals( Left(results[2]), SampleType(1)) &&
			   Equals(Right(results[2]), SampleType(1)));
		assert(Equals( Left(results[3]), SampleType(1.5)) &&
			   Equals(Right(results[3]), SampleType(1.5)));
		assert(Equals( Left(results[4]), SampleType(2)) &&
			   Equals(Right(results[4]), SampleType(2)));
		assert(Equals( Left(results[5]), SampleType(2.5)) &&
			   Equals(Right(results[5]), SampleType(2.5)));
		assert(Equals( Left(results[6]), SampleType(3)) &&
			   Equals(Right(results[6]), SampleType(3)));
	}

	// Test playback speed and change of rate
	{
		Tools::Resampler resam(samples, SAMPLE_RATE * 2);

		StereoData results[4];

		resam.SetPlaybackSpeed(0.5);

		std::generate(
			results, results + (sizeof(results)/sizeof(*results)),
			[& resam]()->StereoData{
				return resam.SendSample();
			}
		);

		assert(Equals( Left(results[0]), SampleType(0)) &&
			   Equals(Right(results[0]), SampleType(0)));
		assert(Equals( Left(results[1]), SampleType(1)) &&
			   Equals(Right(results[1]), SampleType(1)));
		assert(Equals( Left(results[2]), SampleType(2)) &&
			   Equals(Right(results[2]), SampleType(2)));
		assert(Equals( Left(results[3]), SampleType(3)) &&
			   Equals(Right(results[3]), SampleType(3)));
	}
}

static void TestWAVWriter(void)
{
	Track_t track({
		StereoData(SampleType(0), SampleType(0)),
		StereoData(SampleType(0.5), SampleType(0.5)),
		StereoData(SampleType(0), SampleType(0)),
		StereoData(SampleType(-0.5), SampleType(-0.5))
	});

	auto riff = OCAE::Tools::WriteWAV(track);

	RIFF::Reader reader(riff, CONSTRUCT_BYTE_STR("WAVE"));
	auto header = reader.GetChunk(CONSTRUCT_BYTE_STR("fmt "));
	Tools::WAVHeader * h = reinterpret_cast<Tools::WAVHeader*>(header.data());

	assert(h->AudioFormat == 1);
	assert(h->ChannelCount == 2);
	assert(h->SamplingRate == SAMPLE_RATE);
	assert(h->BitsPerSample == 16);
	assert(h->BytesPerSample == h->BitsPerSample / 8 * h->ChannelCount);
	assert(h->BytesPerSecond == SAMPLE_RATE * h->BytesPerSample);

	auto wav_track = reader.GetChunk(CONSTRUCT_BYTE_STR("data"));

	TODO("Check audio data for correctness");
}

static void TestCombinator(void)
{
	std::vector<StereoData> samples{
		StereoData(SampleType( 1), SampleType(1)),
		StereoData(SampleType(-2), SampleType(0))
	};

	// Test addition
	{
		Sound::Combinator comb(Sound::Combinator::Addition);

		StereoData result = comb.Process(samples.begin(), samples.end());

		UNREFERENCED_PARAMETER(result);

		assert(Equals( Left(result), SampleType(-1)));
		assert(Equals(Right(result), SampleType(1 )));
	}

	// Test multiplication
	{
		Sound::Combinator comb(Sound::Combinator::Multiplication);

		StereoData result = comb.Process(samples.begin(), samples.end());

		UNREFERENCED_PARAMETER(result);

		assert(Equals( Left(result), SampleType(-2)));
		assert(Equals(Right(result), SampleType(0 )));
	}
}

// Private Objects                        //////////////////////////////////////

std::vector<VoidFn> tests{
	TestResampler,
	TestWAVWriter,
	TestCombinator
};

// Public Functions                       //////////////////////////////////////

int main(int argc, char * argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	std::cout << "Running tests\n";

		// Start tracking time
	static hrc clk;
	auto before = clk.now();

		// Run all tests
	for(auto & t : tests)
	{
		t();
	}

		// Get the current time
	auto after = clk.now();
		// Calculate the difference between the two times in seconds
	std::chrono::duration<double, std::ratio<1,1>> seconds = after - before;

	std::cout << "Tests completed\n\n";

		// Print the time stats
	std::cout << "Ran all tests in "
	          << std::setprecision(10) << seconds.count()
	          << " seconds\n";

	return 0;
}
