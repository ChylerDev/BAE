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
  Square::Square(Math_t f) : Base(false), m_Ind(0), m_Inv(SAMPLE_RATE/(2*double(f)))
  {
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

  TODO("Once FixedPoint supports operator/, remove the double cast here")
  void Square::SetFrequency(Math_t f)
  {
    m_Inv = SAMPLE_RATE/(2*double(f));
  }

} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
