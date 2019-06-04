/*! ****************************************************************************
\file             Sine.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cmath>

#include "Sine.hpp"

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

  Sine::Sine(Math_t f) : Base(false),
    irate(INC_RATE*double(f)),
    y1(std::sin(double(PI2 * irate))), y2(), beta(),
    m_Table()
  {
    m_Table["SetFrequency"] = [this](void * freq){ SetFrequency(*reinterpret_cast<Math_t*>(freq)); };

    Reset();
  }

  StereoData_t Sine::SendSample(void)
  {
    SampleType_t y = beta * y1 - y2;

    y2 = y1;
    y1 = y;

    return MONO_TO_STEREO(y);
  }

  void Sine::SendBlock(StereoData_t * buffer, uint64_t size)
  {
    static uint64_t i = 0;

    for(i = 0; i < size; ++i)
    {
      static SampleType_t sample;

      sample = beta * y1 - y2;
      y2 = y1;
      y1 = sample;

      static StereoData_t out;
      out = MONO_TO_STEREO(sample);
      std::get<0>(buffer[i]) += std::get<0>(out);
      std::get<1>(buffer[i]) += std::get<1>(out);
    }
  }

  void Sine::SetFrequency(Math_t f)
  {
    irate = INC_RATE * double(f);
    Reset();
  }

  MethodTable_t const & Sine::GetMethodTable() const
  {
    return m_Table;
  }

} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{

  void Sine::Reset()
  {
    beta = 2 * std::cos(double(2*PI * irate));
  }

} // namespace Generator
} // namespace AudioEngine
