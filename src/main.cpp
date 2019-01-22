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

#include "Engine.hpp"

#include "Tools/Input.hpp"
#include "Generators/Sine.hpp"
#include "Generators/WAV.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

int main(int argc, char * argv[])
{
  CreateOptions(argc, argv);

  Generator::Sine sine_data(55);
  Generator::WAV wav_data;
  AudioCallback_t ref;

  if(argc > 1)
  {
    wav_data.ReadFile(GetOptions().at(1));
    ref = std::bind(&Generator::WAV::SendSample, &wav_data);
  }
  else
  {
    ref = std::bind(&Generator::Sine::SendSample, &sine_data);
  }
  Core::Driver driver;

  driver.AddAudioCallback(ref);

  std::cin.get();

  driver.Shutdown();

  return 0;
}

// Private Functions                      //////////////////////////////////////
