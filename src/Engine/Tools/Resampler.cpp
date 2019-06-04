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
    if(size_t(m_Index) >= m_Data.size() && m_LoopEnd == 0)
    {
      return StereoData_t(0,0);
    }

    SampleType_t fraction(m_Index - uint64_t(m_Index));

    SampleType_t l_x1(std::get<0>(m_Data[uint64_t(m_Index)]));
    SampleType_t l_x2(std::get<0>(m_Data[uint64_t(m_Index)+1]));

    SampleType_t r_x1(std::get<1>(m_Data[uint64_t(m_Index)]));
    SampleType_t r_x2(std::get<1>(m_Data[uint64_t(m_Index)+1]));

    SampleType_t l(l_x1 + fraction * (l_x2 - l_x1));
    SampleType_t r(r_x1 + fraction * (r_x2 - r_x1));

    StereoData_t sample(l, r);

    m_Index += m_IndexIncrement;

    if(m_Index >= m_LoopEnd && m_LoopEnd != 0)
    {
      m_Index -= (m_LoopEnd - m_LoopStart);
    }

    return sample;
  }

  void Resampler::SendBlock(StereoData_t * buffer, uint64_t size)
  {
    static uint64_t i;

    for(i = 0; i < size; ++i)
    {
      if(size_t(m_Index) >= m_Data.size() && m_LoopEnd == 0)
      {
        return;
      }

      std::get<0>(buffer[i]) += (std::get<0>(m_Data[uint64_t(m_Index)])) +
                                (m_Index - uint64_t(m_Index)) *
                                ((std::get<0>(m_Data[uint64_t(m_Index)+1])) -
                                 (std::get<0>(m_Data[uint64_t(m_Index)])));
      std::get<1>(buffer[i]) += (std::get<1>(m_Data[uint64_t(m_Index)])) +
                                (m_Index - uint64_t(m_Index)) *
                                ((std::get<1>(m_Data[uint64_t(m_Index)+1])) -
                                 (std::get<1>(m_Data[uint64_t(m_Index)])));
    }
  }

} // namespace Tools
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
