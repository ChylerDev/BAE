/*! ****************************************************************************
\file             Noise.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Noise.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{

  Noise::Noise() : Base(false),
    m_Distribution(-0x8000, 0x7FFF), m_Engine(std::random_device()())
  {
  }

  StereoData_t Noise::SendSample()
  {
    SampleType_t sample;
    sample.Data() = m_Distribution(m_Engine);

    return MONO_TO_STEREO(sample);
  }

} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
