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
#include "../Engine/Modifiers/Gain.hpp"
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
  using Tone_t = AudioEngine::Generator::Sine;

  AudioEngine::Tools::CreateOptions(argc, argv);

  AudioEngine::Driver_t driver = AudioEngine::Core::Driver::Create(1.0);

  #if 0

    AudioEngine::Node_t gen;

    if(argc > 1)
    {
      Log::Trace::out[stc] << "Reading from WAV file\n";
      gen = AudioEngine::Core::Node::Create(AudioEngine::Generator::Base::Create<AudioEngine::Generator::WAV>(AudioEngine::Tools::GetOptions().at(1)));
    }
    else
    {
      Log::Trace::out[stc] << "Generating sine tone\n";
      gen = AudioEngine::Core::Node::Create(AudioEngine::Generator::Base::Create<Tone_t>(440.f));
    }

    AudioEngine::Vocoder_t v = AudioEngine::Sounds::Vocoder::Create(gen, 4);
    driver->AddSound(v->ToSound());

  #elif 1

    AudioEngine::Sound_t sound = AudioEngine::Core::Sound::Create(0.75);

    AudioEngine::Node_t g1 = AudioEngine::Core::Node::Create(AudioEngine::Generator::Base::Create<Tone_t>(220.f));
    AudioEngine::Node_t g2 = AudioEngine::Core::Node::Create(AudioEngine::Generator::Base::Create<Tone_t>(440.f));
    AudioEngine::Node_t g3 = AudioEngine::Core::Node::Create(AudioEngine::Generator::Base::Create<Tone_t>(660.f));
    AudioEngine::Node_t g4 = AudioEngine::Core::Node::Create(AudioEngine::Generator::Base::Create<Tone_t>(880.f));
    AudioEngine::Node_t g5 = AudioEngine::Core::Node::Create(AudioEngine::Generator::Base::Create<Tone_t>(1100.f));
    AudioEngine::Node_t g6 = AudioEngine::Core::Node::Create(AudioEngine::Generator::Base::Create<Tone_t>(1320.f));

    AudioEngine::Node_t m1 = AudioEngine::Core::Node::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.125));
    AudioEngine::Node_t m2 = AudioEngine::Core::Node::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.0625));
    AudioEngine::Node_t m3 = AudioEngine::Core::Node::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.375));
    AudioEngine::Node_t m4 = AudioEngine::Core::Node::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.125));
    AudioEngine::Node_t m5 = AudioEngine::Core::Node::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.0625));
    AudioEngine::Node_t m6 = AudioEngine::Core::Node::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.375));

    g1->AddTarget(*m1);
    g2->AddTarget(*m2);
    g3->AddTarget(*m3);
    g4->AddTarget(*m4);
    g5->AddTarget(*m5);
    g6->AddTarget(*m6);

    sound->AddNode(g1, 0).AddNode(m1, 1, true)
          .AddNode(g2, 0).AddNode(m2, 1, true)
          .AddNode(g3, 0).AddNode(m3, 1, true)
          .AddNode(g4, 0).AddNode(m4, 1, true)
          .AddNode(g5, 0).AddNode(m5, 1, true)
          .AddNode(g6, 0).AddNode(m6, 1, true);

    driver->AddSound(sound);

  #else

    AudioEngine::Sound_t sound = AudioEngine::Core::Sound::Create(1.0);

    AudioEngine::Node_t node1 = AudioEngine::Core::Node::Create(AudioEngine::Generator::Base::Create<Tone_t>(440.f));
    sound->AddNode(node1, 0);

    AudioEngine::Node_t node2 = AudioEngine::Core::Node::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::EnvelopeFollower>(20.f, 20'000.f));
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
