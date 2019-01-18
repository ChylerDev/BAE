/*! ****************************************************************************
\file   main.cpp
\author Chyler Morrison
\email  contact\@chyler.info
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <iostream>
#include <thread>
#include <chrono>

#include "Engine.hpp"

#include "Generators/Sine.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

int main(int argc, char * argv[])
{
  UNREFERENCED_PARAMETER(argc);
  UNREFERENCED_PARAMETER(argv);

  Generator::Sine sine_data(55);
  Driver driver;

  driver.AddAudioCallback(AudioCallback_t(std::bind(&Generator::Sine::GetData, &sine_data)));

  //std::this_thread::sleep_for(std::chrono::seconds(10));
  std::cin.get();

  driver.Shutdown();

  return 0;
}

// Private Functions                      //////////////////////////////////////
