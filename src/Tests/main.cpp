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

#include "../Engine/Core/Sound.hpp"
#include "../Engine/Generators/Sine.hpp"
#include "../Engine/Generators/Square.hpp"
#include "../Engine/Generators/WAV.hpp"
#include "../Engine/Modifiers/Vocoder.hpp"
#include "../Engine/Tools/Input.hpp"
#include "../Engine/Tools/WAVWriter.hpp"

#include <Trace/Trace.hpp>
#include <RIFF-Util/RIFF.hpp>

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

int main(int argc, char * argv[])
{
  AudioEngine::Tools::CreateOptions(argc, argv);

  AudioEngine::Core::Driver driver(0.125f);

  #if 0

    AudioEngine::Generator::Sine sine_data(55);
    std::shared_ptr<AudioEngine::Core::Node> gen;

    if(argc > 1)
    {
      Log::Trace::out[stc] << "Reading from WAV file\n";

      gen = std::make_shared<AudioEngine::Core::Node>(
        std::make_shared<AudioEngine::Generator::WAV>(AudioEngine::Tools::GetOptions().at(1))
      );
    }
    else
    {
      Log::Trace::out[stc] << "Generating sine tone\n";
      gen = std::make_shared<AudioEngine::Core::Node>(
        std::make_shared<AudioEngine::Generator::Sine>(55.f)
      );
    }

    AudioEngine::Modifier::Vocoder v(gen, 4);
    driver.AddSound(v);

  #else

    using Tone_t = AudioEngine::Generator::Square;
    using AudioEngine::Core::Sound;
    using AudioEngine::Core::Node;

    std::shared_ptr<Sound> sound = std::make_shared<Sound>();

    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(220.f)), 0, true);
    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(440.f)), 0, true);
    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(660.f)), 0, true);
    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(880.f)), 0, true);
    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(1100.f)), 0, true);
    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(1320.f)), 0, true);

    driver.AddSound(sound);

  #endif

  driver.StartRecording();

  std::cin.get();

  std::basic_ofstream<RIFF::byte_t> file("wave_out.wav", std::ios_base::binary);
  file << AudioEngine::Tools::WriteWAV(driver.StopRecording());

  driver.Shutdown();

  return 0;
}

// Private Functions                      //////////////////////////////////////
