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

#include "../Engine/Core/Driver.hpp"
#include "../Engine/Core/Node.hpp"
#include "../Engine/Core/Sound.hpp"
#include "../Engine/Generators/Base.hpp"
#include "../Engine/Generators/Sine.hpp"
#include "../Engine/Generators/Square.hpp"
#include "../Engine/Generators/WAV.hpp"
#include "../Engine/Sounds/Vocoder.hpp"
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
  using Tone_t = AudioEngine::Square_t;

  AudioEngine::Tools::CreateOptions(argc, argv);

  AudioEngine::pDriver_t driver = AudioEngine::Driver_t::Create(0.125f);

  #if 1

    AudioEngine::pNode_t gen;

    if(argc > 1)
    {
      Log::Trace::out[stc] << "Reading from WAV file\n";
      gen = AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<AudioEngine::WAV_t>(AudioEngine::Tools::GetOptions().at(1)));
    }
    else
    {
      Log::Trace::out[stc] << "Generating sine tone\n";
      gen = AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(55.f));
    }

    AudioEngine::pVocoder_t v = AudioEngine::Vocoder_t::Create(gen, 4);
    driver->AddSound(v->ToSound());

  #else

    AudioEngine::pSound_t sound = AudioEngine::Sound_t::Create();

    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(220.f)), 0, true);
    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(440.f)), 0, true);
    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(660.f)), 0, true);
    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(880.f)), 0, true);
    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(1100.f)), 0, true);
    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(1320.f)), 0, true);

    driver->AddSound(sound);

  #endif

  driver->StartRecording();

  std::cin.get();

  std::basic_ofstream<RIFF::byte_t> file("wave_out.wav", std::ios_base::binary);
  file << AudioEngine::Tools::WriteWAV(driver->StopRecording());

  driver->Shutdown();

  return 0;
}

// Private Functions                      //////////////////////////////////////
