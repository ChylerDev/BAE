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

  void Noise::SendBlock(StereoData_t * buffer, uint64_t size)
  {
    static uint64_t i = 0;
    for(i = 0; i < size; ++i)
    {
      static SampleType_t sample;

      sample.Data() = m_Distribution(m_Engine);

      static StereoData_t out;
      out = MONO_TO_STEREO(sample);
      std::get<0>(buffer[i]) += std::get<0>(out);
      std::get<1>(buffer[i]) += std::get<1>(out);
    }
  }

} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
