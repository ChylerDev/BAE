/*! ****************************************************************************
\file             Sine.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cmath>

#include "../Engine.hpp"

#include "Square.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{

  TODO("Once FixedPoint supports operator/, remove the double cast here")
  Square::Square(Math_t f) : Base(false),
    m_Ind(0), m_Inv(SAMPLE_RATE/(2*double(f))),
    m_Table()
  {
    m_Table["SetFrequency"] = [this](void * freq){ SetFrequency(*reinterpret_cast<Math_t*>(freq)); };
  }

  StereoData_t Square::SendSample(void)
  {
    double y = 1;

    if(m_Ind >= m_Inv && m_Ind < 2*m_Inv)
    {
      y = -1;
    }

    if(m_Ind >= 2*m_Inv)
    {
      m_Ind -= 2*m_Inv;
    }

    ++m_Ind;

    return MONO_TO_STEREO(y);
  }

  void Square::SendBlock(StereoData_t * buffer, uint64_t size)
  {
    static uint64_t i;

    for(i = 0; i < size; ++i)
    {
      static SampleType_t sample;
      sample = (m_Ind >= m_Inv && m_Ind < 2*m_Inv) ? -1 : 1;

      if(m_Ind >= 2*m_Inv)
      {
        (++m_Ind) -= 2*m_Inv;
      }

      static StereoData_t out;
      out = MONO_TO_STEREO(sample);
      std::get<0>(buffer[i]) += std::get<0>(out);
      std::get<1>(buffer[i]) += std::get<1>(out);
    }
  }

  TODO("Once FixedPoint supports operator/, remove the double cast here")
  void Square::SetFrequency(Math_t f)
  {
    m_Inv = SAMPLE_RATE/(2*double(f));
  }

  MethodTable_t const & Square::GetMethodTable() const
  {
    return m_Table;
  }

} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
