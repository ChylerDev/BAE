/*! ****************************************************************************
\file             Echo.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Echo.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{
	Echo::Echo(
		uint64_t sample_delay,
		Math_t decay_ratio
	) : ModifierBase(false),
		m_Echo(sample_delay, StereoData(SampleType(0), SampleType(0))), m_Ratio(decay_ratio)
	{
	}

	StereoData Echo::FilterSample(StereoData const & dry)
	{
		StereoData wet = m_Echo.front();
		m_Echo.pop_front();

		StereoData out = StereoData(
			SampleType(Left(wet) * m_Ratio + Left(dry)),
			SampleType(Right(wet) * m_Ratio + Right(dry))
		);

		m_Echo.push_back(out);

		return out;
	}

	void Echo::FilterBlock(StereoData * input, StereoData * output, uint64_t size)
	{
		static uint64_t i;

		for(i = 0; i < size; ++i)
		{
			static StereoData out;
			out = StereoData(
				SampleType(Left(m_Echo.front()) * m_Ratio + Left(input[i])),
				SampleType(Right(m_Echo.front()) * m_Ratio + Right(input[i]))
			);

			m_Echo.pop_front();
			m_Echo.push_back(out);

			Left(output[i]) += Left(out);
			Right(output[i]) += Right(out);
		}
	}
} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
