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

//////////////// Tools ////////////////

static void TestMethodTable(void)
{
	class MT : public Tools::MethodTable
	{
	public:
		MT() { RegisterMethods(CreateMethodList()); };
		virtual ~MT() {};

		void OneParam(int i)
		{
			OCAE_UNREFERENCED_PARAMETER(i);
			assert(i == 1);
		};
		int OneRet()
		{
			return 82;
		};
		int OneRetOneParam(int i)
		{
			OCAE_UNREFERENCED_PARAMETER(i);
			assert(i == 2);
			return 239;
		};
		void TwoParam(int i, double d)
		{
			OCAE_UNREFERENCED_PARAMETER(i);
			assert(i == 3);
			OCAE_UNREFERENCED_PARAMETER(d);
			assert(EQUALS(d, 2.5));
		};

	protected:
		MethodList_t CreateMethodList()
		{
			return {
				std::make_tuple(
					std::string("OneParam"),
					Void_fn(
						[this](void * p){
							OneParam(std::get<0>(*reinterpret_cast<std::tuple<OCAE_METHOD_PARAM_T(int)>*>(p)));
						}
					)
				),
				std::make_tuple(
					std::string("OneRet"),
					Void_fn(
						[this](void * p){
							std::get<0>(*reinterpret_cast<std::tuple<OCAE_METHOD_RET_T(int)>*>(p)) = OneRet();
						}
					)
				),
				std::make_tuple(
					std::string("OneRetOneParam"),
					Void_fn(
						[this](void * p){
							auto & t = *reinterpret_cast<std::tuple<OCAE_METHOD_RET_T(int),OCAE_METHOD_PARAM_T(int)>*>(p);
							std::get<0>(t) = OneRetOneParam(std::get<1>(t));
						}
					)
				),
				std::make_tuple(
					std::string("TwoParam"),
					Void_fn(
						[this](void * p){
							auto & t = *reinterpret_cast<std::tuple<OCAE_METHOD_PARAM_T(int),OCAE_METHOD_PARAM_T(double)>*>(p);
							TwoParam(std::get<0>(t), std::get<1>(t));
						}
					)
				)
			};
		}
	};

	MT m;

	m.CallMethod("OneParam", OCAE_METHOD_PARAM(int(1)));
	int r = 0;
	m.CallMethod("OneRet", OCAE_METHOD_RET(r));
	assert(r == 82);
	m.CallMethod("OneRetOneParam", OCAE_METHOD_RET(r), OCAE_METHOD_PARAM(int(2)));
	assert(r == 239);
	m.CallMethod("TwoParam", OCAE_METHOD_PARAM(int(3)), OCAE_METHOD_PARAM(double(2.5)));
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

//////////////// Generators ////////////////

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

	OCAE_WRITE_WAV("noise.wav", samples);

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

//////////////// Modifiers ////////////////

static void TestADSR(void)
{
	auto adsr = Modifier::ModifierFactory::CreateADSR(0.03125, 0.03125, -3, 0.5);
	auto s = Generator::GeneratorFactory::CreateSawtooth(440);

	Track_t t;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		t.push_back(adsr->FilterSample(s->SendSample()));

		if(i == OCAE_SAMPLE_RATE/2)
		{
			adsr->CallMethod("Release");
			// adsr->Release();
		}
	}
	OCAE_WRITE_WAV("adsr_0.03125_0.125_0.5_0.25.sin_440.wav", t);

	totalSamples += OCAE_SAMPLE_RATE;
}

static void TestBandPass(void)
{
	auto bp = Modifier::ModifierFactory::CreateBandPass(100,200);
	auto n = Generator::GeneratorFactory::CreateNoise();

	Track_t t;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		t.push_back(bp->FilterSample(n->SendSample()));
	}
	OCAE_WRITE_WAV("bandpass_100_200.noise.wav", t);

	totalSamples += OCAE_SAMPLE_RATE;
}

static void TestDelay(void)
{
	auto d = Modifier::ModifierFactory::CreateDelay(0.25);
	auto s = Generator::GeneratorFactory::CreateSine(440);

	Track_t t;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		t.push_back(d->FilterSample(s->SendSample()));
	}
	OCAE_WRITE_WAV("delay_0.25.sine_440.wav", t);

	totalSamples += OCAE_SAMPLE_RATE;
}

static void TestEcho(void)
{
	auto e = Modifier::ModifierFactory::CreateEcho(0.125, 0.75);
	auto t = Generator::GeneratorFactory::CreateTriangle(440);

	Track_t tr;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		tr.push_back(e->FilterSample(i<OCAE_SAMPLE_RATE/16?
			t->SendSample() : StereoData()
		));
	}
	OCAE_WRITE_WAV("echo_0.125_0.75.tringale_440.wav", tr);

	totalSamples += OCAE_SAMPLE_RATE;
}

static void TestEnvelope(void)
{
	auto e = Modifier::ModifierFactory::CreateEnvelopeFollower();
	auto s = Generator::GeneratorFactory::CreateSquare(440);
	auto a = Modifier::ModifierFactory::CreateADSR(0.03125, 0.03125, -3, 0.5);

	Track_t t;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		t.push_back(
			e->FilterSample(
				a->FilterSample(
					s->SendSample()
				)
			)
		);

		if(i == OCAE_SAMPLE_RATE/2)
		{
			a->Release();
		}
	}
	OCAE_WRITE_WAV("envelope.adsr.square_440.wav", t);

	totalSamples += OCAE_SAMPLE_RATE;
}

static void TestEqualizer(void)
{
	auto e = Modifier::ModifierFactory::CreateEqualizer();
	auto n = Generator::GeneratorFactory::CreateNoise();

	e->SetGain(0, 0);
	e->SetGain(1, 1);

	Track_t t;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		t.push_back(e->FilterSample(n->SendSample()));

		e->SetGain(0, i*OCAE_INC_RATE);
		e->SetGain(1, 1 - i*OCAE_INC_RATE);
	}
	OCAE_WRITE_WAV("equalizer_2_20_20k.noise.wav", t);

	totalSamples += OCAE_SAMPLE_RATE;
}

static void TestGain(void)
{
	auto g = Modifier::ModifierFactory::CreateGain(0);
	auto s = Generator::GeneratorFactory::CreateSine(440);

	Track_t t;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		t.push_back(g->FilterSample(s->SendSample()));

		if(i < OCAE_SAMPLE_RATE/2)
			g->SetGain(2*i*OCAE_INC_RATE);
		else
			g->SetGain(1 - 2*(i-OCAE_SAMPLE_RATE/2)*OCAE_INC_RATE);
	}
	OCAE_WRITE_WAV("gain.sine_440.wav", t);

	totalSamples += OCAE_SAMPLE_RATE;
}

static void TestGenericFilter(void)
{
	auto g = Modifier::ModifierFactory::CreateGenericFilter(
		{
			std::make_tuple(
				uint32_t(0),
				Math_t(0.69)
			),
			std::make_tuple(
				uint32_t(1),
				Math_t(0.32)
			),
			std::make_tuple(
				uint32_t(2),
				Math_t(0.13)
			),
			std::make_tuple(
				uint32_t(6),
				Math_t(0.4892)
			)
		},
		{
			std::make_tuple(
				uint32_t(1),
				Math_t(0.3789462467)
			),
			std::make_tuple(
				uint32_t(5),
				Math_t(0.12501285032)
			),
			std::make_tuple(
				uint32_t(300),
				Math_t(0.10453659456)
			)
		}
	);

	auto s = Generator::GeneratorFactory::CreateSquare(440);

	Track_t t;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		t.push_back(g->FilterSample(s->SendSample()));
	}
	OCAE_WRITE_WAV("generic.sine_440.wav", t);

	totalSamples += OCAE_SAMPLE_RATE;
}

static void TestLowPass(void)
{
	auto l = Modifier::ModifierFactory::CreateLowPass(440, 0);
	auto n = Generator::GeneratorFactory::CreateNoise();

	Track_t t;
	for(uint64_t i = 0; i < OCAE_SAMPLE_RATE; ++i)
	{
		l->SetResonance(i*OCAE_INC_RATE);

		t.push_back(l->FilterSample(n->SendSample()));
	}
	OCAE_WRITE_WAV("lowpass_440.noise.wav", t);

	totalSamples += OCAE_SAMPLE_RATE;
}

//////////////// Sounds ////////////////

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

	OCAE_WRITE_WAV("sound.sin_440.echo_0.25_0.5.wav", t);

	totalSamples += 20 + OCAE_SAMPLE_RATE*4;
}

//////////////// Core ////////////////

// Private Objects                        //////////////////////////////////////

std::vector<VoidFn> tests{
	TestMethodTable,
	TestResampler,
	TestWAVWriter,
	TestGeneratorBase,
	TestNoise,
	TestSawtooth,
	TestSine,
	TestSquare,
	TestTriangle,
	TestWAV,
	TestADSR,
	TestBandPass,
	TestDelay,
	TestEcho,
	TestEnvelope,
	TestEqualizer,
	TestGain,
	TestGenericFilter,
	TestLowPass,
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
