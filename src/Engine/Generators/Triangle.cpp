/*! ****************************************************************************
\file             Triangle.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Triangle.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Generator
{
	Triangle::Triangle(Math_t freq) : GeneratorBase(),
		m_Irate(4 * double(freq) * INC_RATE), m_Inc()
	{
		SetMethods(CreateMethodList());
	}

	void Triangle::SetFrequency(Math_t freq)
	{
		if(m_Irate < 0) freq *= -1;

		m_Irate = 4 * double(freq) * INC_RATE;
	}

	StereoData Triangle::SendSample(void)
	{
		m_Inc += m_Irate;

		if(m_Inc >= 1 || m_Inc <= -1)
		{
			m_Irate = -m_Irate;

			m_Inc = (m_Inc >= 1) ? (2 - m_Inc) : (-2 - m_Inc);
		}

		return MONO_TO_STEREO(m_Inc);
	}

	std::vector<std::tuple<std::string, Void_fn>> Triangle::CreateMethodList()
	{
		return {
			std::make_tuple(
				std::string("SetFrequency"),
				Void_fn([this](void * f){
					SetFrequency(std::get<0>(*reinterpret_cast<std::tuple<Math_t>*>(f)));
				})
			)
		};
	}
} // namespace Generator
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
