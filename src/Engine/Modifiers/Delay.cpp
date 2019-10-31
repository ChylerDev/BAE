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

namespace OCAE
{
namespace Modifier
{
	Delay::Delay(uint64_t samples) : ModifierBase(),
		m_Delay(samples, StereoData())
	{
		SetMethods(CreateMethodList());
	}

	StereoData Delay::FilterSample(StereoData const & sample)
	{
		m_Delay.push_back(sample);

		StereoData s = m_Delay.front();
		m_Delay.pop_front();

		return s;
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

	std::vector<std::tuple<std::string, Void_fn>> Delay::CreateMethodList()
	{
		return {
			std::make_tuple(
				std::string("SetDelay"),
				Void_fn(
					[this](void * p){
						SetDelay(std::get<0>(*reinterpret_cast<std::tuple<uint64_t>*>(p)));
					}
				)
			)
		};
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
