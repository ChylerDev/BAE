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
		RegisterMethods(CreateMethodList());
	}

	void Triangle::SetFrequency(Math_t freq)
	{
		if(m_Irate < 0) freq *= -1;

		m_Irate = 4 * double(freq) * INC_RATE;
	}

	Math_t Triangle::GetFrequency() const
	{
		return std::abs(m_Irate/(4*INC_RATE));
	}

	StereoData Triangle::SendSample(void)
	{
		StereoData out = MONO_TO_STEREO(m_Inc);
		m_Inc += m_Irate;

		if(m_Inc >= 1 || m_Inc <= -1)
		{
			m_Irate = -m_Irate;

			m_Inc = (m_Inc >= 1) ? (2 - m_Inc) : (-2 - m_Inc);
		}

		return out;
	}

	Tools::MethodTable::MethodList_t Triangle::CreateMethodList()
	{
		return {
			std::make_tuple(
				std::string("SetFrequency"),
				Tools::MethodTable::Void_fn(
					[this](void * f){
						SetFrequency(
							std::get<0>(
								*reinterpret_cast<
									std::tuple<METHOD_PARAM_T(Math_t)>*
								>(f)
							)
						);
					}
				)
			),
			std::make_tuple(
				std::string("GetFrequency"),
				Tools::MethodTable::Void_fn(
					[this](void * f){
						std::get<0>(
							*reinterpret_cast<
								std::tuple<METHOD_RET_T(Math_t)>*
							>(f)
						) = GetFrequency();
					}
				)
			)
		};
	}
} // namespace Generator
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
