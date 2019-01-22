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

namespace Tools
{

  Resampler::Resampler(std::vector<StereoData_t> const & AudioData,
                       float SourceSampleRate,
                       uint64_t LoopStart, uint64_t LoopEnd) :
    m_Data(AudioData), m_Index(0), m_IndexIncrement(SourceSampleRate*INC_RATE),
    m_LoopStart(LoopStart), m_LoopEnd(LoopEnd)
  {
  }

  StereoData_t Resampler::SendSample()
  {
    double l,r;

    l = std::get<0>(m_Data[uint64_t(std::floor(m_Index))]) +
        (m_Index - std::floor(m_Index)) *
        (std::get<0>(m_Data[uint64_t(std::floor(m_Index))+1]) - std::get<0>(m_Data[uint64_t(std::floor(m_Index))]));
    r = std::get<1>(m_Data[uint64_t(std::floor(m_Index))]) +
        (m_Index - std::floor(m_Index)) *
        (std::get<1>(m_Data[uint64_t(std::floor(m_Index))+1]) - std::get<1>(m_Data[uint64_t(std::floor(m_Index))]));

    m_Index += m_IndexIncrement;

    if(m_Index >= m_LoopEnd && m_LoopEnd != 0)
    {
      m_Index -= (m_LoopEnd - m_LoopStart);
    }

    return StereoData_t(float(l), float(r));
  }

} // namespace Tools

// Private Functions                      //////////////////////////////////////
