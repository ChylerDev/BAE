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
    m_Delay(samples, StereoData_t(0,0)), m_Table()
  {
    m_Table["SetDelay"] = [this](void * s){ SetDelay(*reinterpret_cast<uint64_t*>(s)); };
  }

  StereoData_t Delay::FilterSample(StereoData_t const & sample)
  {
    m_Delay.push_back(sample);

    StereoData_t s = m_Delay.front();
    m_Delay.pop_front();

    return s;
  }

  void Delay::SetDelay(uint64_t samples)
  {
    while(samples < m_Delay.size())
    {
      m_Delay.pop_back();
    }

    while(samples > m_Delay.size())
    {
      m_Delay.push_back(StereoData_t(0,0));
    }
  }

  MethodTable_t const & Delay::GetMethodTable() const
  {
    return m_Table;
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
