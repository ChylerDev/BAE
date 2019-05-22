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
    y1(), y2(), beta()
  {
    Reset();
  }

  StereoData_t Sine::SendSample(void)
  {
    SampleType_t y = beta * y1 - y2;

    y2 = y1;
    y1 = y;

    return MONO_TO_STEREO(y);
  }

  void Sine::SetFrequency(Math_t f)
  {
    irate = INC_RATE * double(f);
    Reset();
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
    y1 = std::sin(double(PI2 * irate));
    y2 = 0;
    beta = 2 * std::cos(double(2*PI * irate));
  }

} // namespace Generator
} // namespace AudioEngine
