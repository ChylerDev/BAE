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

namespace AudioEngine
{
namespace Modifier
{
	Delay::Delay(uint64_t samples) : ModifierBase(false),
		m_Delay(samples, StereoData(SampleType(0), SampleType(0))), m_Table()
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

	void Delay::FilterBlock(StereoData * input, StereoData * output, uint64_t size)
	{
		m_Delay.insert(m_Delay.end(), input, input + size);

		static uint64_t i;
		for(i = 0; i < size; ++i)
		{
			Left(output[i]) += Left(m_Delay.front());
			Right(output[i]) += Right(m_Delay.front());

			m_Delay.pop_front();
		}
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

	MethodTable_t const & Delay::GetMethodTable() const
	{
		return m_Table;
	}
} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
