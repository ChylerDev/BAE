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

namespace OCAE
{
namespace Generator
{
	Sawtooth::Sawtooth(Math_t freq) : GeneratorBase(),
		m_Irate(2 * double(freq) * INC_RATE), m_Inc()
	{
		RegisterMethods(CreateMethodList());
	}

	void Sawtooth::SetFrequency(Math_t freq)
	{
		m_Irate = 2 * double(freq) * INC_RATE;
	}

	StereoData Sawtooth::SendSample()
	{
		m_Inc += m_Irate;

		if(m_Inc >= 1)
		{
			m_Inc -= 2;
		}

		return MONO_TO_STEREO(m_Inc);
	}

	Tools::MethodTable::MethodList_t Sawtooth::CreateMethodList()
	{
		return {
			std::make_tuple(
				std::string("SetFrequency"),
				Tools::MethodTable::Void_fn([this](void * f){
					SetFrequency(std::get<0>(*reinterpret_cast<std::tuple<Math_t>*>(f)));
				})
			)
		};
	}
} // namespace Generator
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
