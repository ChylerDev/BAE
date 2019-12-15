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

namespace OCAE
{
namespace Generator
{
	Square::Square(Math_t f) : GeneratorBase(),
		m_Ind(0), m_Inv(SAMPLE_RATE/(2*f))
	{
		RegisterMethods(CreateMethodList());
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

	Math_t Square::GetFrequency() const
	{
		return SAMPLE_RATE/(2*m_Inv);
	}

	Tools::MethodTable::MethodList_t Square::CreateMethodList()
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
