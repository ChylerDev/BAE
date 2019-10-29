/*! ****************************************************************************
\file             Delay.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Delay.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	Delay::Delay(uint64_t samples) : ModifierBase(false),
		m_Delay(samples, StereoData(SampleType(0), SampleType(0)))
	{
		m_Table["SetDelay"] = [this](void * s){ SetDelay(*reinterpret_cast<uint64_t*>(s)); };
	}

	StereoData Delay::FilterSample(StereoData const & sample)
	{
		m_Delay.push_back(sample);

		StereoData s = m_Delay.front();
		m_Delay.pop_front();

		return s;
	}

	void Delay::SetDelay(uint64_t samples)
	{
		while(samples < m_Delay.size())
		{
			m_Delay.pop_back();
		}

		while(samples > m_Delay.size())
		{
			m_Delay.push_back(StereoData(SampleType(0), SampleType(0)));
		}
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
