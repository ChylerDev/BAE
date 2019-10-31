/*! ****************************************************************************
\file             main.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  OCAE

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <iostream>
#include <fstream>
#include <functional>
#include <thread>
#include <chrono>

#include "../Engine/Engine.hpp"
#include "../Engine/Tools/WAVHeader.hpp"

using hrc = std::chrono::high_resolution_clock;

// Private Macros                         //////////////////////////////////////

#define EMPTY_FN [](uint64_t){}

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

static void TestSound(
	std::string filename,
	OCAE::Sound::SoundPtr sound,
	std::function<void(uint64_t)> fn
);

// Public Functions                       //////////////////////////////////////

int main(int argc, char * argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	{
		std::cout << "Simple sine test - 1 second @ 440Hz\n";

		auto sine = OCAE::Sound::SoundFactory::CreateBasicGenerator(
			OCAE::Generator::GeneratorFactory::CreateSine(440)
		);

		TestSound("sine.440.1s.wav", sine, EMPTY_FN);

		std::cout << "Sine test finished\n\n";
	}

	{
		std::cout << "Simple Square wave test - 1 second @ 440Hz\n";

		auto square = OCAE::Sound::SoundFactory::CreateBasicGenerator(
			OCAE::Generator::GeneratorFactory::CreateSquare(440)
		);

		TestSound("square.440.1s.wav", square, EMPTY_FN);

		std::cout << "Square test finished\n\n";
	}

	{
		std::cout << "Square @ 440Hz fed into a Low Pass filter cutoff at 880Hz\n";

		auto sound = OCAE::Sound::SoundFactory::CreateEmptySound();
		auto & graph = sound->GetGraph();

		auto square = OCAE::Sound::SoundFactory::CreateBlock(
				OCAE::Generator::GeneratorFactory::CreateSquare(440)
		);
		auto lp = OCAE::Sound::SoundFactory::CreateBlock(
				OCAE::Modifier::ModifierFactory::CreateLowPass(880, 0)
		);

		auto g = std::deque<OCAE::Sound::Sound::Edge::E_BlockPtr>(
			1, OCAE::Sound::Sound::CreateE_Block(square)
		);
		auto m = std::deque<OCAE::Sound::Sound::Edge::E_BlockPtr>(
			1, OCAE::Sound::Sound::CreateE_Block(lp)
		);

		auto edge = OCAE::Sound::Sound::CreateEdge(
			g,
			OCAE::Sound::Combinator(OCAE::Sound::Combinator::Addition),
			m
		);

		graph.insert(graph.end()-1, edge);
		graph.back()->inputs.push_back(m.front());

		TestSound("square.440.lowpass.880.1s.wav", sound, EMPTY_FN);

		std::cout << "Filtered square test finished\n\n";
	}

	{
		std::cout << "Noise filtered at 400Hz test\n";

		auto sound = OCAE::Sound::SoundFactory::CreateEmptySound();
		auto & graph = sound->GetGraph();

		auto noise = OCAE::Sound::SoundFactory::CreateBlock(
				OCAE::Generator::GeneratorFactory::CreateNoise()
		);
		auto lp = OCAE::Sound::SoundFactory::CreateBlock(
				OCAE::Modifier::ModifierFactory::CreateLowPass(400, 0)
		);

		auto g = std::deque<OCAE::Sound::Sound::Edge::E_BlockPtr>(
			1, OCAE::Sound::Sound::CreateE_Block(noise)
		);
		auto m = std::deque<OCAE::Sound::Sound::Edge::E_BlockPtr>(
			1, OCAE::Sound::Sound::CreateE_Block(lp)
		);

		auto edge = OCAE::Sound::Sound::CreateEdge(
			g,
			OCAE::Sound::Combinator(OCAE::Sound::Combinator::Addition),
			m
		);

		graph.insert(graph.end()-1, edge);
		graph.back()->inputs.push_back(m.front());

		TestSound("noise.lowpass.400.1s.wav", sound, EMPTY_FN);

		std::cout << "Filtered noise test finished\n\n";
	}

	{
		std::cout << "Sine @ 440 Hz with ADSR test\n";

		auto sine_adsr = OCAE::Sound::SoundFactory::CreateEmptySound();
		auto & graph = sine_adsr->GetGraph();

		auto sine = OCAE::Sound::SoundFactory::CreateBlock(
			OCAE::Generator::GeneratorFactory::CreateSine(440)
		);
		auto adsr = OCAE::Sound::SoundFactory::CreateBlock(
			OCAE::Modifier::ModifierFactory::CreateADSR(
				uint64_t(0.05*SAMPLE_RATE),
				uint64_t(0.1*SAMPLE_RATE),
				0.3,
				uint64_t(0.4*SAMPLE_RATE)
			)
		);

		auto sine_block = OCAE::Sound::Sound::CreateE_Block(sine);
		auto adsr_block = OCAE::Sound::Sound::CreateE_Block(adsr);
		auto edge = OCAE::Sound::Sound::CreateEdge(
			std::deque<OCAE::Sound::Sound::Edge::E_BlockPtr>(1, sine_block),
			OCAE::Sound::Combinator(OCAE::Sound::Combinator::Addition),
			std::deque<OCAE::Sound::Sound::Edge::E_BlockPtr>(1, adsr_block)
		);

		graph.insert(graph.end()-1, edge);
		graph.back()->inputs.push_back(adsr_block);

		TestSound(
			"sine.440.adsr.wav",
			sine_adsr,
			[&, adsr](uint64_t i){
				if(i == 60){
					adsr->GetModifier()->CallMethod("Release");
				}
			}
		);

		std::cout << "ADSR Sine test finished\n\n";
	}

	return 0;
}

// Private Functions                      //////////////////////////////////////

static void TestSound(std::string filename, OCAE::Sound::SoundPtr sound, std::function<void(uint64_t)> fn)
{
		// Initialize objects that should live for the life of the test
	static OCAE::Core::DriverPtr driver = OCAE::Core::DriverPtr(new OCAE::Core::Driver(MAX_BUFFER));
	static OCAE::Track_t const & driver_output = driver->GetOutputTrack();
	static OCAE::Track_t output(SAMPLE_RATE, OCAE::StereoData());
	static hrc clk;

		// Ensure the output track is clear before running the test
	output.clear();

		// Register the provided sound with the driver
	OCAE::Sound::Sound::Register(sound, driver);
		// Unpause the sound, in case it was paused
	sound->Unpause();

		// Get the current time
	auto before = clk.now();
		// Loop for one second's worth of samples
	for(uint64_t i = 0; i < SAMPLE_RATE/MAX_BUFFER; ++i)
	{
			// Process this loop's samples
		driver->Process();
			// Copy the generated samples into the output buffer
		output.insert(output.end(), driver_output.begin(), driver_output.end());

			// Call the given function with the current index
		fn(i);
	}
		// Get the current time
	auto after = clk.now();
		// Calculate the difference between the two times
	auto difference = after - before;
		// Print the time stats
	std::cout << "Generated 1 second of audio in "
	          << (double(difference.count()) / 1'000'000'000.0)
	          << " seconds\n";

		// Generate wave data
	auto WAVData = OCAE::Tools::WriteWAV(output);

		// Open a file with the given filename
	std::ofstream WAVFile(filename, std::ios_base::binary);

		// Write the contents of the wave data to the file and close
	WAVFile.write(reinterpret_cast<char const*>(WAVData.data()), WAVData.size());
	WAVFile.flush();
	WAVFile.close();

		// Pause the sound so that it isn't playing when being unregistered from the driver
	sound->Pause();
		// Unregister the provided sound from the driver
	OCAE::Sound::Sound::Unregister(sound);
}
