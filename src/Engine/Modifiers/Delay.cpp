/*! ****************************************************************************
\file             Delay.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Delay.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{

  Delay::Delay(uint64_t samples) : Base(false),
    m_Delay(samples, StereoData_t(0,0))
  {
  }

  StereoData_t Delay::FilterSample(StereoData_t const & sample)
  {
    m_Delay.push_back(sample);

    StereoData_t s = m_Delay.front();
    m_Delay.pop_front();

    return s;
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
