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
		m_Distribution(-1.f, 1.f), m_Engine(std::random_device()())
	{
		RegisterMethods(CreateMethodList());
	}

	StereoData Noise::SendSample()
	{
		SampleType sample;
		sample = m_Distribution(m_Engine);

		return OCAE_MONO_TO_STEREO(sample);
	}
} // namespace Generator
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
