/*! ****************************************************************************
\file             ADSR.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "ADSR.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{

  TODO("Fix double casts")
  ADSR::ADSR(uint64_t a, uint64_t d, Math_t s, uint64_t r) : Base(false),
    m_Attack(1.0/a), m_Decay(double(s-1)/d), m_Sustain(s), m_Release(double(-s)/r),
    m_State(state::attack), m_Gain(0)
  {
  }

  StereoData_t ADSR::FilterSample(StereoData_t const & sample)
  {
    switch(m_State)
    {
      case state::attack:
        m_Gain += m_Attack;
        if(m_Gain >= 1)
        {
          m_State = state::decay;
          m_Gain = 1;
        }
        break;
      case state::decay:
        m_Gain += m_Decay;
        if(m_Gain <= m_Sustain)
        {
          m_State = state::sustain;
          m_Gain = m_Sustain;
        }
        break;
      case state::sustain:
        break;
      case state::release:
        m_Gain += m_Release;
        if(m_Gain <= 0)
        {
          m_State = state::invalid;
          m_Gain = 0;
        }
        break;
      default:
        return StereoData_t(0,0);
        break;
    };
    return StereoData_t(
      SampleType_t(std::get<0>(sample) * m_Gain),
      SampleType_t(std::get<1>(sample) * m_Gain)
    );
  }

  void ADSR::Release()
  {
    m_State = state::release;
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
