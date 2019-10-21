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
#include "../Engine/Core/Block.hpp"
#include "../Engine/Core/Sound.hpp"
#include "../Engine/Generators/GeneratorBase.hpp"
#include "../Engine/Generators/Noise.hpp"
#include "../Engine/Generators/Sawtooth.hpp"
#include "../Engine/Generators/Sine.hpp"
#include "../Engine/Generators/Square.hpp"
#include "../Engine/Generators/Triangle.hpp"
#include "../Engine/Generators/WAV.hpp"
#include "../Engine/Modifiers/ModifierBase.hpp"
#include "../Engine/Modifiers/ADSR.hpp"
#include "../Engine/Modifiers/Delay.hpp"
#include "../Engine/Modifiers/Echo.hpp"
#include "../Engine/Modifiers/Envelope.hpp"
#include "../Engine/Modifiers/Gain.hpp"
#include "../Engine/Tools/Input.hpp"
#include "../Engine/Tools/WAVWriter.hpp"

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

    AudioEngine::Block_t gen;

    if(argc > 1)
    {
      std::cout << "Reading from WAV file\n";
      gen = AudioEngine::Core::Block::Create(AudioEngine::Generator::Base::Create<AudioEngine::Generator::WAV>(AudioEngine::Tools::GetOptions().at(1)));
    }
    else
    {
      std::cout << "Generating sine tone\n";
      gen = AudioEngine::Core::Block::Create(AudioEngine::Generator::Base::Create<Tone_t>(440.f));
    }

    AudioEngine::Vocoder_t v = AudioEngine::Sounds::Vocoder::Create(gen, 4);
    driver->AddSound(v->ToSound());

  #elif 0

    AudioEngine::Sound_t sound = AudioEngine::Core::Sound::Create(0.75);

    AudioEngine::Block_t g1 = AudioEngine::Core::Block::Create(AudioEngine::Generator::Base::Create<Tone_t>(220.f));
    AudioEngine::Block_t g2 = AudioEngine::Core::Block::Create(AudioEngine::Generator::Base::Create<Tone_t>(440.f));
    AudioEngine::Block_t g3 = AudioEngine::Core::Block::Create(AudioEngine::Generator::Base::Create<Tone_t>(660.f));
    AudioEngine::Block_t g4 = AudioEngine::Core::Block::Create(AudioEngine::Generator::Base::Create<Tone_t>(880.f));
    AudioEngine::Block_t g5 = AudioEngine::Core::Block::Create(AudioEngine::Generator::Base::Create<Tone_t>(1100.f));
    AudioEngine::Block_t g6 = AudioEngine::Core::Block::Create(AudioEngine::Generator::Base::Create<Tone_t>(1320.f));

    AudioEngine::Block_t m1 = AudioEngine::Core::Block::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.125));
    AudioEngine::Block_t m2 = AudioEngine::Core::Block::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.0625));
    AudioEngine::Block_t m3 = AudioEngine::Core::Block::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.375));
    AudioEngine::Block_t m4 = AudioEngine::Core::Block::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.125));
    AudioEngine::Block_t m5 = AudioEngine::Core::Block::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.0625));
    AudioEngine::Block_t m6 = AudioEngine::Core::Block::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::Gain>(0.375));

    sound->AddBlock(g1, 0).AddBlock(m1, 1, true)
          .SetTarget(g1, 0, m1, 0)
          .AddBlock(g2, 0).AddBlock(m2, 1, true)
          .SetTarget(g2, 0, m2, 0)
          .AddBlock(g3, 0).AddBlock(m3, 1, true)
          .SetTarget(g3, 0, m3, 0)
          .AddBlock(g4, 0).AddBlock(m4, 1, true)
          .SetTarget(g4, 0, m4, 0)
          .AddBlock(g5, 0).AddBlock(m5, 1, true)
          .SetTarget(g5, 0, m5, 0)
          .AddBlock(g6, 0).AddBlock(m6, 1, true)
          .SetTarget(g6, 0, m6, 0);

    driver->AddSound(sound);

  #elif 0

    AudioEngine::Sound_t sound = AudioEngine::Core::Sound::Create(1.0);

    AudioEngine::Block_t block1 = AudioEngine::Core::Block::Create(AudioEngine::Generator::Base::Create<Tone_t>(440.f));
    sound->AddBlock(block1, 0, true);

    // AudioEngine::Block_t block2 = AudioEngine::Core::Block::Create(AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::EnvelopeFollower>(20.f, 20'000.f));
    // sound->AddBlock(block2, 1, true)
    //       .SetTarget(block1, 0, block2, 1);

    driver->AddSound(sound);

  #else

    AudioEngine::Sound_t sound = AudioEngine::Core::Sound::Create(1.0);

    AudioEngine::Block_t n1 = AudioEngine::Core::Block::Create(
      AudioEngine::Generator::Base::Create<Tone_t>(440)
    );
    auto mod = AudioEngine::Modifier::Base::Create<AudioEngine::Modifier::ADSR>(
      uint64_t(0.0375*SAMPLE_RATE), uint64_t(0.20*SAMPLE_RATE), AudioEngine::Math_t(0.5), uint64_t(0.25*SAMPLE_RATE)
    );
    AudioEngine::Block_t n2 = AudioEngine::Core::Block::Create(mod);

    sound->AddBlock(n1, 0)
          .AddBlock(n2, 1, true)
          .SetTarget(n1, 0, n2, 1);

    driver->AddSound(sound);

  #endif

  driver->StartRecording();

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  METHOD(*n1->GetGenerator(), SetFrequency, AudioEngine::Math_t, 110);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  METHOD(*n2->GetModifier(), Release, void *, nullptr);

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  driver->Shutdown();

  std::ofstream file("wave_out.wav", std::ios_base::binary);
  if(!file)
  {
    std::cerr << "Error in opening file for write\n";
  }
  else
  {
    auto r = AudioEngine::Tools::WriteWAV(driver->StopRecording());

    file.write(reinterpret_cast<char*>(r.data()), r.size());
  }

  return 0;
}

// Private Functions                      //////////////////////////////////////
