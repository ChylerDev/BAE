/*! ****************************************************************************
\file             Resampler.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Resampler.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Tools
{
	Resampler::Resampler(std::vector<StereoData> const & AudioData,
						 int32_t SourceSampleRate,
						 uint64_t LoopStart, uint64_t LoopEnd) :
		m_Data(AudioData), m_Index(0), m_IndexIncrement(SourceSampleRate*INC_RATE),
		m_PlaybackSpeed(1.0), m_LoopStart(LoopStart), m_LoopEnd(LoopEnd)
	{
	}

	void Resampler::SetPlaybackSpeed(Math_t playback)
	{
		m_PlaybackSpeed = playback;
	}

	StereoData Resampler::SendSample()
	{
		if(size_t(m_Index) >= m_Data.size() && m_LoopEnd == 0)
		{
			return StereoData(SampleType(0), SampleType(0));
		}

		SampleType fraction(SampleType(m_Index - Math_t(uint64_t(m_Index))));

		SampleType l_x1(Left(m_Data[uint64_t(m_Index)]));
		SampleType l_x2(Left(m_Data[uint64_t(m_Index)+1]));

		SampleType r_x1(Right(m_Data[uint64_t(m_Index)]));
		SampleType r_x2(Right(m_Data[uint64_t(m_Index)+1]));

		SampleType l(l_x1 + fraction * (l_x2 - l_x1));
		SampleType r(r_x1 + fraction * (r_x2 - r_x1));

		StereoData sample(l, r);

		m_Index += m_IndexIncrement * m_PlaybackSpeed;

		if(m_Index >= Math_t(m_LoopEnd) && m_LoopEnd != 0)
		{
			m_Index -= Math_t(m_LoopEnd - m_LoopStart);
		}

		return sample;
	}
} // namespace Tools
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
