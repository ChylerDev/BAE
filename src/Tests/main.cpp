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
	AudioEngine::Tools::InitOptions(argc, argv);

	AudioEngine::Core::DriverPtr driver = AudioEngine::Core::DriverPtr(new AudioEngine::Core::Driver(MAX_BUFFER));
	AudioEngine::Track_t output;

	std::cout << "Simple sine test - 1 second @ 440Hz\n";

	driver->AddSound(
		AudioEngine::Sound::SoundFactory::CreateBasicGenerator(
			AudioEngine::Generator::GeneratorFactory::CreateSine(440)
		)
	);

	AudioEngine::Track_t const & driver_output = driver->GetOutputTrack();

	for(uint64_t i = 0; i < SAMPLE_RATE/MAX_BUFFER; ++i)
	{
		driver->Process();

		output.insert(output.end(), driver_output.begin(), driver_output.end());
	}

	auto WAVData = AudioEngine::Tools::WriteWAV(output);

	std::ofstream WAVFile("sine.440.1s.wav", std::ios_base::binary);

	WAVFile.write(reinterpret_cast<char const*>(WAVData.data()), WAVData.size());
	WAVFile.flush();
	WAVFile.close();

	std::cout << "Sine test finished\n";

	return 0;
}

// Private Functions                      //////////////////////////////////////
