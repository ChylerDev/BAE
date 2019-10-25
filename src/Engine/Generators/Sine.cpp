/*! ****************************************************************************
\file             Sine.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
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
	Sine::Sine(Math_t f) : GeneratorBase(false),
		irate(INC_RATE*double(f)),
		y1(SampleType(std::sin(double(PI2 * irate)))), y2(), beta()
	{
		m_Table["SetFrequency"] = [this](void * freq){ SetFrequency(*reinterpret_cast<Math_t*>(freq)); };

		Reset();
	}

	StereoData Sine::SendSample(void)
	{
		SampleType y = SampleType(beta * y1 - y2);

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
		beta = Math_t(2 * std::cos(double(2*PI * irate)));
	}
} // namespace Generator
} // namespace AudioEngine
