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

namespace AudioEngine
{
namespace Generator
{
	Noise::Noise() : GeneratorBase(false),
		m_Distribution(-0x8000, 0x7FFF), m_Engine(std::random_device()())
	{
	}

	StereoData_t Noise::SendSample()
	{
		SampleType_t sample;
		sample = m_Distribution(m_Engine)/SampleType_t(0x8000);

		return MONO_TO_STEREO(sample);
	}

	void Noise::SendBlock(StereoData_t * buffer, uint64_t size)
	{
		static uint64_t i = 0;
		for(i = 0; i < size; ++i)
		{
			static SampleType_t sample;

			sample = m_Distribution(m_Engine)/SampleType_t(0x8000);

			static StereoData_t out;
			out = MONO_TO_STEREO(sample);
			Left(buffer[i]) += Left(out);
			Right(buffer[i]) += Right(out);
		}
	}
} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
