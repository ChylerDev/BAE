/*! ****************************************************************************
\file             Gain.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Gain.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	Gain::Gain(Math_t gain) : ModifierBase(false), m_Gain(gain)
	{
		m_Table["SetGain"] = [this](void * g){ SetGain(*reinterpret_cast<Math_t*>(g)); };
		m_Table["GetGain"] = [this](void * g){ *reinterpret_cast<Math_t*>(g) = GetGain(); };
	}

	void Gain::SetGain(Math_t gain)
	{
		m_Gain = gain;
	}

	Math_t Gain::GetGain() const
	{
		return m_Gain;
	}

	StereoData Gain::FilterSample(StereoData const & input)
	{
		return StereoData(SampleType(Math_t( Left(input)) * m_Gain),
						  SampleType(Math_t(Right(input)) * m_Gain));
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
