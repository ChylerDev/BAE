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
    m_Attack(1.0/double(a)), m_Decay(double(s-1)/double(d)),
    m_Sustain(s), m_Release(double(-s)/double(r)),
    m_State(state::attack), m_Gain(0),
    m_Table()
  {
    m_Table["Release"] = [this](void *){ Release(); };
  }

  void ADSR::Release()
  {
    m_State = state::release;
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
      case state::invalid:
      default:
        return StereoData_t(0,0);
        break;
    };
    return StereoData_t(
      SampleType_t(std::get<0>(sample) * m_Gain),
      SampleType_t(std::get<1>(sample) * m_Gain)
    );
  }

  void ADSR::FilterBlock(StereoData_t * input, StereoData_t * output, uint64_t size)
  {
    static uint64_t i;

    for(i = 0; i < size; ++i)
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
        case state::invalid:
        default:
          return;
          break;
      };

      std::get<0>(output[i]) += std::get<0>(input[i]) * m_Gain;
      std::get<1>(output[i]) += std::get<1>(input[i]) * m_Gain;
    }
  }

  MethodTable_t const & ADSR::GetMethodTable() const
  {
    return m_Table;
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////