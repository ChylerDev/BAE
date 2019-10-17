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

  Gain::Gain(Math_t gain) : Base(false), m_Gain(gain), m_Table()
  {
    m_Table["SetGain"] = [this](void * g){ SetGain(*reinterpret_cast<Math_t*>(g)); };
    m_Table["GetGain"] = [this](void * g){ *reinterpret_cast<Math_t*>(g) = GetGain(); };
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
    return StereoData_t(SampleType_t(Left(input) * m_Gain),
                        SampleType_t(Right(input) * m_Gain));
  }

  void Gain::FilterBlock(StereoData_t * input, StereoData_t * output, uint64_t size)
  {
    static uint64_t i;

    for(i = 0; i < size; ++i)
    {
      Left(output[i]) += SampleType_t(Left(input[i]) * m_Gain);
      Right(output[i]) += SampleType_t(Right(input[i]) * m_Gain);
    }
  }

  MethodTable_t const & Gain::GetMethodTable() const
  {
    return m_Table;
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
