/*! ****************************************************************************
\file             Sawtooth.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Sawtooth.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{
	Sawtooth::Sawtooth(Math_t freq) : GeneratorBase(false),
		m_Irate(2 * double(freq) * INC_RATE), m_Inc(), m_Table()
	{
		m_Table["SetFrequency"] = [this](void * f){ SetFrequency(*reinterpret_cast<Math_t*>(f)); };
	}

	void Sawtooth::SetFrequency(Math_t freq)
	{
		m_Irate = 2 * double(freq) * INC_RATE;
	}

	StereoData_t Sawtooth::SendSample()
	{
		m_Inc += m_Irate;

		if(m_Inc >= 1)
		{
			m_Inc -= 2;
		}

		return MONO_TO_STEREO(m_Inc);
	}

	void Sawtooth::SendBlock(StereoData_t * buffer, uint64_t size)
	{
		static uint64_t i = 0;

		for(i = 0; i < size; ++i)
		{
		m_Inc += m_Irate;

		if(m_Inc >= 1)
		{
			m_Inc -= 2;
		}

		static StereoData_t out;
		out = MONO_TO_STEREO(m_Inc);
		Left(buffer[i]) += Left(out);
		Right(buffer[i]) += Right(out);
		}
	}

	MethodTable_t const & Sawtooth::GetMethodTable() const
	{
		return m_Table;
	}
} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
