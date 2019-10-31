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
	Gain::Gain(Math_t gain) : ModifierBase(), m_Gain(gain)
	{
		SetMethods(CreateMethodList());
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

	std::vector<std::tuple<std::string, Void_fn>> Gain::CreateMethodList()
	{
		return {
			std::make_tuple(
				std::string("SetGain"),
				Void_fn(
					[this](void * p){
						SetGain(std::get<0>(*reinterpret_cast<std::tuple<Math_t>*>(p)));
					}
				)
			),
			std::make_tuple(
				std::string("GetGain"),
				Void_fn(
					[this](void * p){
						std::get<0>(*reinterpret_cast<std::tuple<Math_t>*>(p)) = GetGain();
					}
				)
			)
		};
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
