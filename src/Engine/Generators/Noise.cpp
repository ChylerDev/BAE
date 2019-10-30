/*! ****************************************************************************
\file             Noise.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Noise.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Generator
{
	Noise::Noise() : GeneratorBase(),
		m_Distribution(-0x8000, 0x7FFF), m_Engine(std::random_device()())
	{
	}

	StereoData Noise::SendSample()
	{
		SampleType sample;
		sample = m_Distribution(m_Engine)/SampleType(0x8000);

		return MONO_TO_STEREO(sample);
	}
} // namespace Generator
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
