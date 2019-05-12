/*! ****************************************************************************
\file             main.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <iostream>
#include <thread>
#include <chrono>

#include "../Engine/Engine.hpp"

#include "../Engine/Tools/Input.hpp"
#include "../Engine/Generators/Sine.hpp"
#include "../Engine/Generators/Square.hpp"
#include "../Engine/Generators/WAV.hpp"

#include "../Libraries/Logging/src/Trace.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

int main(int argc, char * argv[])
{
  AudioEngine::Tools::CreateOptions(argc, argv);

  #if 1

    AudioEngine::Generator::Sine sine_data(55);
    AudioEngine::Generator::WAV wav_data;
    AudioCallback_t ref;

    if(argc > 1)
    {
      Log::Trace::out[stc] << "Reading from WAV file\n";
      wav_data.ReadFile(AudioEngine::Tools::GetOptions().at(1));
      ref = std::bind(&AudioEngine::Generator::WAV::SendSample, &wav_data);
    }
    else
    {
      Log::Trace::out[stc] << "Generating sine tone\n";
      ref = std::bind(&AudioEngine::Generator::Sine::SendSample, &sine_data);
    }

    AudioEngine::Core::Driver driver;

    driver.AddAudioCallback(ref);

  #else

  AudioEngine::Generator::Square root(110);
  AudioEngine::Generator::Square first(220);
  AudioEngine::Generator::Square second(440);
  AudioEngine::Generator::Square third(880);
  AudioEngine::Generator::Square fourth(1760);
  AudioEngine::Generator::Square fifth(3520);

  AudioCallback_t ref_r([&root](){ return root.SendSample(); });
  AudioCallback_t ref_1([&first](){ return first.SendSample(); });
  AudioCallback_t ref_2([&second](){ return second.SendSample(); });
  AudioCallback_t ref_3([&third](){ return third.SendSample(); });
  AudioCallback_t ref_4([&fourth](){ return fourth.SendSample(); });
  AudioCallback_t ref_5([&fifth](){ return fifth.SendSample(); });

  AudioEngine::Core::Driver driver;

  driver.AddAudioCallback(ref_r);
  driver.AddAudioCallback(ref_1);
  driver.AddAudioCallback(ref_2);
  driver.AddAudioCallback(ref_3);
  driver.AddAudioCallback(ref_4);
  driver.AddAudioCallback(ref_5);

  #endif

  std::cin.get();

  driver.Shutdown();

  return 0;
}

// Private Functions                      //////////////////////////////////////
