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

  Sine::Sine(float f) : irate(INC_RATE*double(f)), y1(std::sin(2*PI*irate)), y2(0), beta(2*std::cos(2*PI*irate))
  {
  }

  StereoData_t Sine::SendSample(void)
  {
    double y = beta * y1 - y2;

    y2 = y1;
    y1 = y;

    return MONO_TO_STEREO(y);
  }

  void Sine::SetFrequency(float f)
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
    y1 = std::sin(PI2 * irate);
    y2 = 0;
    beta = 2 * std::cos(2*PI * irate);
  }

} // namespace Generator
} // namespace AudioEngine
