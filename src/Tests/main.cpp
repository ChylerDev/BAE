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
#include "../Engine/Modifiers/Base.hpp"
#include "../Engine/Modifiers/Envelope.hpp"
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
  using Tone_t = AudioEngine::Sine_t;

  AudioEngine::Tools::CreateOptions(argc, argv);

  AudioEngine::pDriver_t driver = AudioEngine::Driver_t::Create(1.0f);

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
      gen = AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(440.f));
    }

    AudioEngine::pVocoder_t v = AudioEngine::Vocoder_t::Create(gen, 4);
    driver->AddSound(v->ToSound());

  #elif 0

    AudioEngine::pSound_t sound = AudioEngine::Sound_t::Create(1);

    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(220.f)), 0, true);
    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(440.f)), 0, true);
    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(660.f)), 0, true);
    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(880.f)), 0, true);
    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(1100.f)), 0, true);
    sound->AddNode(AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(1320.f)), 0, true);

    driver->AddSound(sound);

  #else

    AudioEngine::pSound_t sound = AudioEngine::Sound_t::Create(1.0);

    AudioEngine::pNode_t node1 = AudioEngine::Node_t::Create(AudioEngine::GenBase_t::Create<Tone_t>(440.f));
    sound->AddNode(node1, 0);

    AudioEngine::pNode_t node2 = AudioEngine::Node_t::Create(AudioEngine::ModBase_t::Create<AudioEngine::Envelope_t>(20.f, 20'000.f));
    node1->AddTarget(*node2);
    sound->AddNode(node2, 1, true);

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
