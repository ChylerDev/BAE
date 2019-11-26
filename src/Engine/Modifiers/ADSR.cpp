/*! ****************************************************************************
\file             ADSR.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "ADSR.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	ADSR::ADSR(uint64_t a, uint64_t d, Math_t s, uint64_t r) : ModifierBase(),
		m_Attack(1.0/Math_t(a)), m_Decay((s-1)/Math_t(d)),
		m_Sustain(s), m_Release(-s/Math_t(r)),
		m_State(State::attack), m_Gain(0)
	{
		RegisterMethods(CreateMethodList());
	}

	void ADSR::Release()
	{
		m_State = State::release;
	}

	StereoData ADSR::FilterSample(StereoData const & sample)
	{
		switch(m_State)
		{
			case State::attack:
				m_Gain += m_Attack;
				if(m_Gain >= 1)
				{
					m_State = State::decay;
					m_Gain = 1;
				}
				break;
			case State::decay:
				m_Gain += m_Decay;
				if(m_Gain <= m_Sustain)
				{
					m_State = State::sustain;
					m_Gain = m_Sustain;
				}
				break;
			case State::sustain:
				break;
			case State::release:
				m_Gain += m_Release;
				if(m_Gain <= 0)
				{
					m_State = State::invalid;
					m_Gain = 0;
				}
				break;
			case State::invalid:
			default:
				return StereoData(SampleType(0), SampleType(0));
				break;
		};
		return StereoData(
			SampleType(Math_t( Left(sample)) * m_Gain),
			SampleType(Math_t(Right(sample)) * m_Gain)
		);
	}

	Tools::MethodTable::MethodList_t ADSR::CreateMethodList()
	{
		return {
			std::make_tuple(
				std::string("Release"),
				Tools::MethodTable::Void_fn([this](void *){ Release(); })
			)
		};
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
