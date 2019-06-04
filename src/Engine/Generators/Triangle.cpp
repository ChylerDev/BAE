/*! ****************************************************************************
\file             stub.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Triangle.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{

  Triangle::Triangle(Math_t freq) : Base(false),
    m_Irate(4 * double(freq) * INC_RATE), m_Inc(), m_Table()
  {
    m_Table["SetFrequency"] = [this](void * f){ SetFrequency(*reinterpret_cast<Math_t*>(f)); };
  }

  void Triangle::SetFrequency(Math_t freq)
  {
    if(m_Irate < 0) freq *= -1;

    m_Irate = 4 * double(freq) * INC_RATE;
  }

  StereoData_t Triangle::SendSample(void)
  {
    m_Inc += m_Irate;

    if(m_Inc >= 1 || m_Inc <= -1)
    {
      m_Irate = -m_Irate;

      m_Inc = (m_Inc >= 1) ? (2 - m_Inc) : (-2 - m_Inc);
    }

    return MONO_TO_STEREO(m_Inc);
  }

  void Triangle::SendBlock(StereoData_t * buffer, uint64_t size)
  {
    static uint64_t i;

    for(i = 0; i < size; ++i)
    {
      m_Inc += m_Irate;

      if(m_Inc >= 1 || m_Inc <= -1)
      {
        m_Irate = -m_Irate;

        m_Inc = (m_Inc >= 1) ? (2-m_Inc) : (-2-m_Inc);
      }

      static StereoData_t out;
      out = MONO_TO_STEREO(m_Inc);
      std::get<0>(buffer[i]) += std::get<0>(out);
      std::get<1>(buffer[i]) += std::get<1>(out);
    }
  }

  MethodTable_t const & Triangle::GetMethodTable() const
  {
    return m_Table;
  }

} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
