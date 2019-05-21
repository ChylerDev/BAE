/*! ****************************************************************************
\file             Resampler.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Resampler.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{

  Resampler::Resampler(std::vector<StereoData_t> const & AudioData,
                       int32_t SourceSampleRate,
                       uint64_t LoopStart, uint64_t LoopEnd) :
    m_Data(AudioData), m_Index(0), m_IndexIncrement(SourceSampleRate*INC_RATE),
    m_LoopStart(LoopStart), m_LoopEnd(LoopEnd)
  {
  }

  StereoData_t Resampler::SendSample()
  {
    StereoData_t sample(
      SampleType_t(std::get<0>(m_Data[uint64_t(m_Index)]) +
        (m_Index - uint64_t(m_Index)) *
        (std::get<0>(m_Data[uint64_t(m_Index)+1]) - std::get<0>(m_Data[uint64_t(m_Index)]))),
      SampleType_t(std::get<1>(m_Data[uint64_t(m_Index)]) +
        (m_Index - uint64_t(m_Index)) *
        (std::get<1>(m_Data[uint64_t(m_Index)+1]) - std::get<1>(m_Data[uint64_t(m_Index)])))
    );

    m_Index += m_IndexIncrement;

    if(m_Index >= m_LoopEnd && m_LoopEnd != 0)
    {
      m_Index -= (m_LoopEnd - m_LoopStart);
    }

    return sample;
  }

} // namespace Tools
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
