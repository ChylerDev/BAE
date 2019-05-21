/*! ****************************************************************************
\file             Gain.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Gain.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{

  Gain::Gain(Math_t gain) : Base(false), m_Gain(gain)
  {
  }

  void Gain::SetGain(Math_t gain)
  {
    m_Gain = gain;
  }

  Math_t Gain::GetGain() const
  {
    return m_Gain;
  }

  StereoData_t Gain::FilterSample(StereoData_t const & input)
  {
    return StereoData_t(SampleType_t(std::get<0>(input) * m_Gain),
                        SampleType_t(std::get<1>(input) * m_Gain));
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
