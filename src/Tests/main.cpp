/*! ****************************************************************************
\file             main.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#include "../Engine/Engine.hpp"
#include "../Engine/Tools/WAVHeader.hpp"

#include <RIFF-Util/RIFF.hpp>

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

int main(int argc, char * argv[])
{
	using clock = std::chrono::high_resolution_clock;

	AudioEngine::Tools::InitOptions(argc, argv);

	AudioEngine::Core::DriverPtr driver = AudioEngine::Core::DriverPtr(new AudioEngine::Core::Driver(MAX_BUFFER));
	AudioEngine::Track_t output;
	output.reserve(SAMPLE_RATE);
	clock clk;

	{
		std::cout << "Simple sine test - 1 second @ 440Hz\n";

		auto SineID = driver->AddSound(
			AudioEngine::Sound::SoundFactory::CreateBasicGenerator(
				AudioEngine::Generator::GeneratorFactory::CreateSine(440)
			)
		);

		AudioEngine::Track_t const & driver_output = driver->GetOutputTrack();

		auto before = clk.now();
		for(uint64_t i = 0; i < SAMPLE_RATE/MAX_BUFFER; ++i)
		{
			driver->Process();

			output.insert(output.end(), driver_output.begin(), driver_output.end());
		}
		auto after = clk.now();
		auto difference = after - before;
		std::cout << "Generated 1 second of audio in "
				  << (double(difference.count()) / 1'000'000'000.0)
				  << " seconds\n";

		auto WAVData = AudioEngine::Tools::WriteWAV(output);

		std::ofstream WAVFile("sine.440.1s.wav", std::ios_base::binary);

		WAVFile.write(reinterpret_cast<char const*>(WAVData.data()), WAVData.size());
		WAVFile.flush();
		WAVFile.close();

		driver->RemoveSound(SineID);

		std::cout << "Sine test finished\n";
	}

	{
		std::cout << "Simple Square wave test - 1 second @ 440Hz\n";

		auto SquareID = driver->AddSound(
			AudioEngine::Sound::SoundFactory::CreateBasicGenerator(
				AudioEngine::Generator::GeneratorFactory::CreateSquare(440)
			)
		);

		AudioEngine::Track_t const & driver_output = driver->GetOutputTrack();

		auto before = clk.now();
		for(uint64_t i = 0; i < SAMPLE_RATE/MAX_BUFFER; ++i)
		{
			driver->Process();

			output.insert(output.end(), driver_output.begin(), driver_output.end());
		}
		auto after = clk.now();
		auto difference = after - before;
		std::cout << "Generated 1 second of audio in "
				  << (double(difference.count()) / 1'000'000'000.0)
				  << " seconds\n";

		auto WAVData = AudioEngine::Tools::WriteWAV(output);

		std::ofstream WAVFile("square.440.1s.wav", std::ios_base::binary);

		WAVFile.write(reinterpret_cast<char const*>(WAVData.data()), WAVData.size());
		WAVFile.flush();
		WAVFile.close();

		driver->RemoveSound(SquareID);

		std::cout << "Square test finished\n";
	}

	{
		std::cout << "Square @ 440Hz fed into a Low Pass filter cutoff at 880Hz\n";

		auto sound = AudioEngine::Sound::SoundFactory::CreateEmptySound();
		auto & graph = sound->GetGraph();

		auto square = AudioEngine::Sound::SoundFactory::CreateBlock(
				AudioEngine::Generator::GeneratorFactory::CreateSquare(440)
		);
		auto lp = AudioEngine::Sound::SoundFactory::CreateBlock(
				AudioEngine::Modifier::ModifierFactory::CreateLowPass(880, 0)
		);

		auto g = std::deque<AudioEngine::Sound::Sound::Edge::E_BlockPtr>(
			1, AudioEngine::Sound::Sound::CreateE_Block(square)
		);
		auto m = std::deque<AudioEngine::Sound::Sound::Edge::E_BlockPtr>(
			1, AudioEngine::Sound::Sound::CreateE_Block(lp)
		);

		auto edge = AudioEngine::Sound::Sound::CreateEdge(
			g,
			AudioEngine::Sound::Combinator(AudioEngine::Sound::Combinator::Addition),
			m
		);

		graph.insert(graph.end()-1, edge);
		graph.back()->inputs.push_back(m.front());

		auto LPID = driver->AddSound(sound);

		AudioEngine::Track_t const & driver_output = driver->GetOutputTrack();

		auto before = clk.now();
		for(uint64_t i = 0; i < SAMPLE_RATE/MAX_BUFFER; ++i)
		{
			driver->Process();

			output.insert(output.end(), driver_output.begin(), driver_output.end());
		}
		auto after = clk.now();
		auto difference = after - before;
		std::cout << "Generated 1 second of audio in "
				  << (double(difference.count()) / 1'000'000'000.0)
				  << " seconds\n";

		auto WAVData = AudioEngine::Tools::WriteWAV(output);

		std::ofstream WAVFile("square.440.lowpass.880.1s.wav", std::ios_base::binary);

		WAVFile.write(reinterpret_cast<char const*>(WAVData.data()), WAVData.size());
		WAVFile.flush();
		WAVFile.close();

		driver->RemoveSound(LPID);

		std::cout << "Filtered square test finished\n";
	}

	return 0;
}

// Private Functions                      //////////////////////////////////////
