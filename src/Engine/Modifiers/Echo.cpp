/*! ****************************************************************************
\file             Echo.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Echo.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{

  Echo::Echo(
    uint64_t sample_delay,
    Math_t decay_ratio
  ) : Base(false),
    m_Echo(sample_delay, StereoData_t(0,0)), m_Ratio(decay_ratio)
  {
  }

  StereoData_t Echo::FilterSample(StereoData_t const & dry)
  {
    StereoData_t wet = m_Echo.front();
    m_Echo.pop_front();

    StereoData_t out = StereoData_t(
      std::get<0>(wet) * m_Ratio + std::get<0>(dry),
      std::get<1>(wet) * m_Ratio + std::get<1>(dry)
    );

    m_Echo.push_back(out);

    return out;
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
