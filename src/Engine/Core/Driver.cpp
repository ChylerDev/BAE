/*! ****************************************************************************
\file             Driver.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cstring>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include "Driver.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Core
{
	Driver::Driver(uint64_t track_size, Math_t gain) :
		m_OutputTrack(track_size, StereoData(SampleType(0),SampleType(0))), m_Sounds(), m_Gain(gain)
	{
		m_OutputTrack.reserve(track_size);
	}

	Driver::~Driver()
	{
	}

	void Driver::AddSound(Sound::SoundPtr const & sound)
	{
		m_Sounds.push_back(sound);
	}

	void Driver::SetGain(Math_t gain)
	{
		m_Gain = gain;
	}

	Track_t const & Driver::GetOutputTrack() const
	{
		return m_OutputTrack;
	}

	void Driver::Process()
	{
			// Reset output track
		std::fill(m_OutputTrack.begin(), m_OutputTrack.end(), StereoData(SampleType(0),SampleType(0)));

		for(auto & sample: m_OutputTrack)
		{
			for(auto & sound: m_Sounds)
			{
				sound->PrimeInput(StereoData(SampleType(0), SampleType(0)));
				sound->Process();
				StereoData out = sound->LastOutput();

				 Left(sample) += Left(out);
				Right(sample) += Right(out);
			}

			 Left(sample) = SampleType(Math_t( Left(sample)) * m_Gain);
			Right(sample) = SampleType(Math_t(Right(sample)) * m_Gain);
		}
	}
} // namespace Core
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
