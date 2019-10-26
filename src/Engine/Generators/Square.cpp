/*! ****************************************************************************
\file             Sine.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cmath>

#include "../Engine.hpp"

#include "Square.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{
	Square::Square(Math_t f) : GeneratorBase(false),
		m_Ind(0), m_Inv(SAMPLE_RATE/(2*f))
	{
		m_Table["SetFrequency"] = [this](void * freq){ SetFrequency(*reinterpret_cast<Math_t*>(freq)); };
	}

	StereoData Square::SendSample(void)
	{
		double y = 1;

		if(m_Ind >= m_Inv && m_Ind < 2*m_Inv)
		{
			y = -1;
		}

		if(m_Ind >= 2*m_Inv)
		{
			m_Ind -= 2*m_Inv;
		}

		++m_Ind;

		return MONO_TO_STEREO(y);
	}

	void Square::SetFrequency(Math_t f)
	{
		m_Inv = SAMPLE_RATE/(2*f);
	}
} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
