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
#include <limits>
#include <thread>
#include <chrono>
#include <type_traits>

#include <RIFF-Util/RIFF.hpp>

#include "../Engine/Macro.hpp"
#include "../Engine/Engine.hpp"

using hrc = std::chrono::high_resolution_clock;
using namespace OCAE;
using VoidFn = void(*)(void);

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

#define Equals(a,b) bool(double(std::abs(a-b)) < double(EPSILON_F))
#define WRITEWAV(file, samples) auto _r = Tools::WriteWAV(samples); std::ofstream(file, std::ios_base::binary).write(reinterpret_cast<char *>(_r.data()), std::streamsize(_r.size()))

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
			results, results + SIZEOF_ARRAY(results),
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
			results, results + SIZEOF_ARRAY(results),
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
			results, results + SIZEOF_ARRAY(results),
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
			results, results + SIZEOF_ARRAY(results),
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
	assert(h->BytesPerSecond == uint32_t(SAMPLE_RATE * h->BytesPerSample));

	auto wav_track = reader.GetChunk(CONSTRUCT_BYTE_STR("data"));

	assert(wav_track[0] == 0 && wav_track[1] == 0 &&
		   wav_track[2] == 0 && wav_track[3] == 0);

	assert(wav_track[4] == RIFF::byte_t(uint16_t(0x8000 * 0.5) & 0x00'FF) &&
		   wav_track[5] == RIFF::byte_t((uint16_t(0x8000 * 0.5) & 0xFF'00) >> 8) &&
		   wav_track[6] == RIFF::byte_t(uint16_t(0x8000 * 0.5) & 0x00'FF) &&
		   wav_track[7] == RIFF::byte_t((uint16_t(0x8000 * 0.5) & 0xFF'00) >> 8));
	assert(wav_track[8] == 0 && wav_track[9] == 0 &&
		   wav_track[10] == 0 && wav_track[11] == 0);

	assert(wav_track[12] == RIFF::byte_t((static_cast<uint16_t>(int16_t(0x8000 * -0.5)) & 0x00'FF)) &&
		   wav_track[13] == RIFF::byte_t((static_cast<uint16_t>(int16_t(0x8000 * -0.5)) & 0xFF'00) >> 8) &&
		   wav_track[14] == RIFF::byte_t((static_cast<uint16_t>(int16_t(0x8000 * -0.5)) & 0x00'FF)) &&
		   wav_track[15] == RIFF::byte_t((static_cast<uint16_t>(int16_t(0x8000 * -0.5)) & 0xFF'00) >> 8));
}

static void TestGeneratorBase(void)
{
	auto b = Generator::GeneratorFactory::CreateBase();
	assert(b->IsBase());

	StereoData samples[SAMPLE_RATE/10] = {};
	std::generate(samples, samples + SIZEOF_ARRAY(samples),
				  [& b](){return b->SendSample();});

	for(auto it = samples; it != samples + SIZEOF_ARRAY(samples); ++it)
	{
		assert(Equals(Left(*it),SampleType(0)) && Equals(Right(*it),SampleType(0)));
	}
}

static void TestNoise(void)
{
	auto n = Generator::GeneratorFactory::CreateNoise();
	assert(!n->IsBase());

	Track_t samples;
	for(uint64_t i = 0; i < SAMPLE_RATE; ++i)
	{
		samples.push_back(n->SendSample());
	}

	WRITEWAV("noise.1s.wav", samples);
}

static void TestSawtooth(void)
{
	auto s = Generator::GeneratorFactory::CreateSawtooth(440);
	assert(!s->IsBase());

	Math_t f;
	s->CallMethod("GetFrequency", METHOD_RET(f));
	assert(Equals(f, 440.0));

	StereoData samples[4];
	for(uint64_t i = 0; i < SIZEOF_ARRAY(samples); ++i)
	{
		samples[i] = s->SendSample();
	}

	assert(Equals(Left(samples[0]), SampleType(440*INC_RATE*2*0*SQRT_HALF)));
	assert(Equals(Left(samples[0]), Right(samples[0])));
	assert(Equals(Left(samples[1]), SampleType(440*INC_RATE*2*1*SQRT_HALF)));
	assert(Equals(Left(samples[1]), Right(samples[1])));
	assert(Equals(Left(samples[2]), SampleType(440*INC_RATE*2*2*SQRT_HALF)));
	assert(Equals(Left(samples[2]), Right(samples[2])));
	assert(Equals(Left(samples[3]), SampleType(440*INC_RATE*2*3*SQRT_HALF)));
	assert(Equals(Left(samples[3]), Right(samples[3])));
}

static void TestSine(void)
{
	auto s = Generator::GeneratorFactory::CreateSine(440);
	assert(!s->IsBase());

	Math_t f;
	s->CallMethod("GetFrequency", METHOD_RET(f));
	assert(Equals(f, 440.0));

	StereoData samples[4];
	for(uint64_t i = 0; i < SIZEOF_ARRAY(samples); ++i)
	{
		samples[i] = s->SendSample();
	}

	assert(Equals(Left(samples[0]), SampleType(std::sin(440*PI2*INC_RATE*0))));
	assert(Equals(Left(samples[0]), Right(samples[0])));
	assert(Equals(Left(samples[1]), SampleType(std::sin(440*PI2*INC_RATE*1)*SQRT_HALF)));
	assert(Equals(Left(samples[1]), Right(samples[1])));
	assert(Equals(Left(samples[2]), SampleType(std::sin(440*PI2*INC_RATE*2)*SQRT_HALF)));
	assert(Equals(Left(samples[2]), Right(samples[2])));
	assert(Equals(Left(samples[3]), SampleType(std::sin(440*PI2*INC_RATE*3)*SQRT_HALF)));
	assert(Equals(Left(samples[3]), Right(samples[3])));
}

static void TestSquare(void)
{
	auto s = Generator::GeneratorFactory::CreateSquare(440);
	assert(!s->IsBase());

	Math_t f;
	s->CallMethod("GetFrequency", METHOD_RET(f));
	assert(Equals(f, 440.0));

	StereoData samples[4];
	for(uint64_t i = 0; i < SIZEOF_ARRAY(samples); ++i)
	{
		samples[i] = s->SendSample();
	}

	assert(Equals(Left(samples[0]), SampleType(SQRT_HALF)));
	assert(Equals(Left(samples[0]), Right(samples[0])));
	assert(Equals(Left(samples[1]), SampleType(SQRT_HALF)));
	assert(Equals(Left(samples[1]), Right(samples[1])));
	assert(Equals(Left(samples[2]), SampleType(SQRT_HALF)));
	assert(Equals(Left(samples[2]), Right(samples[2])));
	assert(Equals(Left(samples[3]), SampleType(SQRT_HALF)));
	assert(Equals(Left(samples[3]), Right(samples[3])));
}

static void TestTriangle(void)
{
	auto t = Generator::GeneratorFactory::CreateTriangle(440);
	assert(!t->IsBase());

	Math_t f;
	t->CallMethod("GetFrequency", METHOD_RET(f));
	assert(Equals(f, 440.0));

	StereoData samples[4];
	for(uint64_t i = 0; i < SIZEOF_ARRAY(samples); ++i)
	{
		samples[i] = t->SendSample();
	}

	assert(Equals(Left(samples[0]), SampleType(4*440*SQRT_HALF*INC_RATE*0)));
	assert(Equals(Left(samples[0]), Right(samples[0])));
	assert(Equals(Left(samples[1]), SampleType(4*440*SQRT_HALF*INC_RATE*1)));
	assert(Equals(Left(samples[1]), Right(samples[1])));
	assert(Equals(Left(samples[2]), SampleType(4*440*SQRT_HALF*INC_RATE*2)));
	assert(Equals(Left(samples[2]), Right(samples[2])));
	assert(Equals(Left(samples[3]), SampleType(4*440*SQRT_HALF*INC_RATE*3)));
	assert(Equals(Left(samples[3]), Right(samples[3])));
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
		assert(Equals(Right(result), SampleType( 0)));
	}
}

// Private Objects                        //////////////////////////////////////

std::vector<VoidFn> tests{
	TestResampler,
	TestWAVWriter,
	TestGeneratorBase,
	TestNoise,
	TestSawtooth,
	TestSine,
	TestSquare,
	TestTriangle,
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
