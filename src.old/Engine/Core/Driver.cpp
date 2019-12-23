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

namespace OCAE
{
namespace Core
{
	uint64_t Driver::s_IDCounter = 0;
} // namespace Core
} // namespace OCAE

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Core
{
	Driver::Driver(uint64_t track_size, Math_t gain) :
		m_OutputTrack(track_size, StereoData()),
		m_Sounds(), m_Gain(gain)
	{
		m_OutputTrack.reserve(track_size);
	}

	Driver::~Driver()
	{
	}

	uint64_t Driver::AddSound(Sound::SoundPtr const & sound)
	{
		uint64_t id = GetID();
		m_Sounds[id] = sound;
		return id;
	}

	Sound::SoundPtr Driver::RemoveSound(uint64_t id)
	{
		Sound::SoundPtr sound = m_Sounds[id];
		m_Sounds.erase(id);
		return sound;
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
		std::fill(m_OutputTrack.begin(), m_OutputTrack.end(), StereoData());

		for(auto & sample: m_OutputTrack)
		{
			for(auto & sound: m_Sounds)
			{
				StereoData out = sound.second->Process(StereoData());

				 Left(sample) += Left(out);
				Right(sample) += Right(out);
			}

			 Left(sample) = SampleType(Math_t( Left(sample)) * m_Gain);
			Right(sample) = SampleType(Math_t(Right(sample)) * m_Gain);
		}
	}
} // namespace Core
} // namespace OCAE

// Private Functions                      //////////////////////////////////////

namespace OCAE
{
namespace Core
{
	uint64_t Driver::GetID()
	{
		return s_IDCounter++;
	}
} // namespace Core
} // namespace OCAE
