/*! ****************************************************************************
\file             main.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  OCAE

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cassert>

#include <iomanip>
#include <chrono>
#include <iostream>

#include <RIFF-Util/RIFF.hpp>

#include "../Engine/Macro.hpp"
#include "../Engine/Engine.hpp"

using hrc = std::chrono::high_resolution_clock;
using namespace OCAE;
using VoidFn = void(*)(void);

static hrc clk;

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

#define EQUALS(a,b) bool(double(std::abs(a-b)) < double(OCAE_EPSILON_F))

// Private Functions                      //////////////////////////////////////

static uint64_t totalSamples = 0;

std::ostream & operator<<(std::ostream & o, StereoData const & s)
{
	o << '(' << Left(s) << ',' << Right(s) << ')';
	return o;
}

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
		Tools::Resampler resam(samples, OCAE_SAMPLE_RATE/2);

		StereoData results[7];

		std::generate(
			results, results + OCAE_SIZEOF_ARRAY(results),
			[& resam]()->StereoData{
				return resam.SendSample();
			}
		);

		for(uint64_t i = 0; i < 7; ++i)
		{
			assert(EQUALS( Left(results[i]), SampleType(Math_t(i)/2)) &&
			       EQUALS(Right(results[i]), SampleType(Math_t(i)/2)));
		}

		totalSamples += OCAE_SIZEOF_ARRAY(results);
	}

	// Test decrease of rate
	{
		Tools::Resampler resam(samples, OCAE_SAMPLE_RATE * 2);

		StereoData results[2];

		std::generate(
			results, results + OCAE_SIZEOF_ARRAY(results),
			[& resam]()->StereoData{
				return resam.SendSample();
			}
		);

		assert(EQUALS( Left(results[0]), SampleType(0)) &&
			   EQUALS(Right(results[0]), SampleType(0)));
		assert(EQUALS( Left(results[1]), SampleType(2)) &&
			   EQUALS(Right(results[1]), SampleType(2)));

		totalSamples += OCAE_SIZEOF_ARRAY(results);
	}

	// Test playback speed
	{
		Tools::Resampler resam(samples, OCAE_SAMPLE_RATE);

		StereoData results[8];

		resam.SetPlaybackSpeed(0.5);

		std::generate(
			results, results + OCAE_SIZEOF_ARRAY(results),
			[& resam]()->StereoData{
				return resam.SendSample();
			}
		);

		for(uint64_t i = 0; i < 7; ++i)
		{
			assert(EQUALS( Left(results[i]), SampleType(Math_t(i)/2)) &&
			       EQUALS(Right(results[i]), SampleType(Math_t(i)/2)));
		}

		totalSamples += OCAE_SIZEOF_ARRAY(results);
	}

	// Test playback speed and change of rate
	{
		Tools::Resampler resam(samples, OCAE_SAMPLE_RATE * 2);

		StereoData results[4];

		resam.SetPlaybackSpeed(0.5);

		std::generate(
			results, results + OCAE_SIZEOF_ARRAY(results),
			[& resam]()->StereoData{
				return resam.SendSample();
			}
		);

		for(uint64_t i = 0; i < 4; ++i)
		{
			assert(EQUALS( Left(results[i]), SampleType(i)) &&
			       EQUALS(Right(results[i]), SampleType(i)));
		}

		totalSamples += OCAE_SIZEOF_ARRAY(results);
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
	assert(h->SamplingRate == OCAE_SAMPLE_RATE);
	assert(h->BitsPerSample == 16);
	assert(h->BytesPerSample == h->BitsPerSample / 8 * h->ChannelCount);
	assert(h->BytesPerSecond == uint32_t(OCAE_SAMPLE_RATE * h->BytesPerSample));

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

	OCAE_UNREFERENCED_PARAMETER(h);
}

static void TestGeneratorBase(void)
{
	auto b = Generator::GeneratorFactory::CreateBase();
	assert(b->IsBase());

	StereoData samples[OCAE_SAMPLE_RATE/10] = {};
	std::generate(samples, samples + OCAE_SIZEOF_ARRAY(samples),
				  [& b](){return b->SendSample();});

	for(auto it = samples; it != samples + OCAE_SIZEOF_ARRAY(samples); ++it)
	{
		assert(EQUALS(Left(*it),SampleType(0)) && EQUALS(Right(*it),SampleType(0)));
	}

		totalSamples += OCAE_SIZEOF_ARRAY(samples);
}

static void TestNoise(void)
{
	auto n = Generator::GeneratorFactory::CreateNoise();
	assert(!n->IsBase());

	Track_t samples;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		samples.push_back(n->SendSample());
	}

	OCAE_WRITE_WAV("noise.1s.wav", samples);


	totalSamples += samples.size();
}

static void TestSawtooth(void)
{
	auto s = Generator::GeneratorFactory::CreateSawtooth(440);
	assert(!s->IsBase());

	Math_t f;
	s->CallMethod("GetFrequency", OCAE_METHOD_RET(f));
	assert(EQUALS(f, 440.0));

	for(uint64_t i = 0; i < 4; ++i)
	{
		StereoData sam = s->SendSample();
		assert(EQUALS(Left(sam), SampleType(440*OCAE_INC_RATE*2*i*OCAE_SQRT_HALF)));
		assert(EQUALS(Left(sam), Right(sam)));
	}

	totalSamples += 4;
}

static void TestSine(void)
{
	auto s = Generator::GeneratorFactory::CreateSine(440);
	assert(!s->IsBase());

	Math_t f;
	s->CallMethod("GetFrequency", OCAE_METHOD_RET(f));
	assert(EQUALS(f, 440.0));

	for(uint64_t i = 0; i < 4; ++i)
	{
		StereoData sam = s->SendSample();
		assert(EQUALS(Left(sam), SampleType(std::sin(440*OCAE_PI2*OCAE_INC_RATE*i)*OCAE_SQRT_HALF)));
		assert(EQUALS(Left(sam), Right(sam)));
	}

	totalSamples += 4;
}

static void TestSquare(void)
{
	auto s = Generator::GeneratorFactory::CreateSquare(440);
	assert(!s->IsBase());

	Math_t f;
	s->CallMethod("GetFrequency", OCAE_METHOD_RET(f));
	assert(EQUALS(f, 440.0));

	for(uint64_t i = 0; i < 4; ++i)
	{
		StereoData sam = s->SendSample();
		assert(EQUALS(Left(sam), SampleType(OCAE_SQRT_HALF)));
		assert(EQUALS(Left(sam), Right(sam)));
	}

	totalSamples += 4;
}

static void TestTriangle(void)
{
	auto t = Generator::GeneratorFactory::CreateTriangle(440);
	assert(!t->IsBase());

	Math_t f;
	t->CallMethod("GetFrequency", OCAE_METHOD_RET(f));
	assert(EQUALS(f, 440.0));

	for(uint64_t i = 0; i < 4; ++i)
	{
		StereoData sam = t->SendSample();
		assert(EQUALS(Left(sam), SampleType(4*440*OCAE_SQRT_HALF*OCAE_INC_RATE*i)));
		assert(EQUALS(Left(sam), Right(sam)));
	}

	totalSamples += 4;
}

static void TestWAV(void)
{
	auto s = Generator::GeneratorFactory::CreateSine(440);
	Track_t t;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		t.push_back(s->SendSample());
	}
	RIFF::vector_t riff = Tools::WriteWAV(t);

	auto w = Generator::GeneratorFactory::CreateWAV(
		std::vector<char>(reinterpret_cast<char *>(riff.data()),
		reinterpret_cast<char *>(riff.data()) + riff.size())
	);

	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		StereoData sam = s->SendSample();
		assert(EQUALS(Left(sam), SampleType(std::sin(440*OCAE_PI2*OCAE_INC_RATE*i)*OCAE_SQRT_HALF)));
		assert(EQUALS(Left(sam), Right(sam)));
	}

	totalSamples += OCAE_SAMPLE_RATE;
}

static void TestSound(void)
{
		// Create empty sound object
	auto s1 = Sound::SoundFactory::CreateEmptySound();

		// Check empty sound, should return 0
	for(uint64_t i = 0; i < 16; ++i)
	{
		StereoData sam = s1->Process(StereoData());

		assert(EQUALS(Left(sam), SampleType(0)));
		assert(EQUALS(Left(sam), Right(sam)));

		OCAE_UNREFERENCED_PARAMETER(sam);
	}

		// Create sound containing sine generator
	auto s2 = Sound::SoundFactory::CreateBasicGenerator(
		Generator::GeneratorFactory::CreateSine(440)
	);

	for(uint64_t i = 0; i < 4; ++i)
	{
		StereoData sam = s2->Process(StereoData());
		assert(EQUALS(Left(sam), SampleType(std::sin(440*OCAE_PI2*OCAE_INC_RATE*i)*OCAE_SQRT_HALF)));
		assert(EQUALS(Left(sam), Right(sam)));
	}

		// Create sound
	auto echo = Sound::SoundFactory::CreateEmptySound();
		// Create blocks
	auto delay = Sound::SoundFactory::CreateBlock(
		Modifier::ModifierFactory::CreateDelay(0.25)
	);
	auto gain = Sound::SoundFactory::CreateBlock(
		Modifier::ModifierFactory::CreateGain(OCAE_DEFAULT_GAIN)
	);
		// Connect blocks
	echo->AddConnection(echo->GetInputBlock(), echo->GetOutputBlock());
	echo->AddConnection(echo->GetInputBlock(), delay);
	echo->AddConnection(delay, gain);
	echo->AddConnection(gain, delay);
	echo->AddConnection(gain, echo->GetOutputBlock());

	Track_t t;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE*4; ++i)
	{
		t.push_back(echo->Process(OCAE_MONO_TO_STEREO(std::sin(OCAE_PI2*440*OCAE_INC_RATE*double(i)) * 0.5)));
	}

	OCAE_WRITE_WAV("sound.sin.440.echo.0.25s.0.5g.wav", t);

	totalSamples += 20 + OCAE_SAMPLE_RATE*4;
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
	TestWAV,
	TestSound,
};

// Public Functions                       //////////////////////////////////////

int main(int argc, char * argv[])
{
	OCAE_UNREFERENCED_PARAMETER(argc);
	OCAE_UNREFERENCED_PARAMETER(argv);

	std::cout << "Running tests\n";

		// Start tracking time
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

	double time = seconds.count();

		// Print the time stats
	std::cout << "Ran all tests in "
	          << std::setprecision(10) << time
	          << " seconds\n";

	std::cout << "Computed " << totalSamples << " samples across all tests for an average of "
	          << totalSamples/time << " samples of audio generated per second or "
	          << totalSamples/time*OCAE_INC_RATE << " seconds of audio generated per second.\n";

	return 0;
}
