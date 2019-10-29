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
#include <thread>
#include <chrono>

#include "../Engine/Engine.hpp"
#include "../Engine/Tools/WAVHeader.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

int main(int argc, char * argv[])
{
	using clock = std::chrono::high_resolution_clock;

	OCAE::Tools::InitOptions(argc, argv);

	OCAE::Core::DriverPtr driver = OCAE::Core::DriverPtr(new OCAE::Core::Driver(MAX_BUFFER));
	OCAE::Track_t output;
	output.reserve(SAMPLE_RATE);
	clock clk;

	{
		std::cout << "Simple sine test - 1 second @ 440Hz\n";

		auto sine = OCAE::Sound::SoundFactory::CreateBasicGenerator(
			OCAE::Generator::GeneratorFactory::CreateSine(440)
		);
		OCAE::Sound::Sound::Register(sine, driver);
		sine->Unpause();

		OCAE::Track_t const & driver_output = driver->GetOutputTrack();

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

		auto WAVData = OCAE::Tools::WriteWAV(output);

		std::ofstream WAVFile("sine.440.1s.wav", std::ios_base::binary);

		WAVFile.write(reinterpret_cast<char const*>(WAVData.data()), WAVData.size());
		WAVFile.flush();
		WAVFile.close();
		output.clear();

		OCAE::Sound::Sound::Unregister(sine);

		std::cout << "Sine test finished\n\n";
	}

	{
		std::cout << "Simple Square wave test - 1 second @ 440Hz\n";

		auto square = OCAE::Sound::SoundFactory::CreateBasicGenerator(
			OCAE::Generator::GeneratorFactory::CreateSquare(440)
		);
		OCAE::Sound::Sound::Register(square, driver);
		square->Unpause();

		OCAE::Track_t const & driver_output = driver->GetOutputTrack();

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

		auto WAVData = OCAE::Tools::WriteWAV(output);

		std::ofstream WAVFile("square.440.1s.wav", std::ios_base::binary);

		WAVFile.write(reinterpret_cast<char const*>(WAVData.data()), WAVData.size());
		WAVFile.flush();
		WAVFile.close();
		output.clear();

		OCAE::Sound::Sound::Unregister(square);

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

		OCAE::Sound::Sound::Register(sound, driver);
		sound->Unpause();

		OCAE::Track_t const & driver_output = driver->GetOutputTrack();

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

		auto WAVData = OCAE::Tools::WriteWAV(output);

		std::ofstream WAVFile("square.440.lowpass.880.1s.wav", std::ios_base::binary);

		WAVFile.write(reinterpret_cast<char const*>(WAVData.data()), WAVData.size());
		WAVFile.flush();
		WAVFile.close();
		output.clear();

		OCAE::Sound::Sound::Unregister(sound);

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

		OCAE::Sound::Sound::Register(sound, driver);
		sound->Unpause();

		OCAE::Track_t const & driver_output = driver->GetOutputTrack();

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

		auto WAVData = OCAE::Tools::WriteWAV(output);

		std::ofstream WAVFile("noise.lowpass.400.1s.wav", std::ios_base::binary);

		WAVFile.write(reinterpret_cast<char const*>(WAVData.data()), WAVData.size());
		WAVFile.flush();
		WAVFile.close();
		output.clear();

		OCAE::Sound::Sound::Unregister(sound);

		std::cout << "Filtered noise test finished\n\n";
	}

	return 0;
}

// Private Functions                      //////////////////////////////////////
