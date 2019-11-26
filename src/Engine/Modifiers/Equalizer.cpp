/*! ****************************************************************************
\file             Equalizer.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Equalizer.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	Equalizer::Equalizer(uint32_t band_count, Math_t lower, Math_t upper): ModifierBase(),
		m_BandGains(band_count, Math_t(1)), m_Bands()
	{
		RegisterMethods(CreateMethodList());

		Math_t const delta = (std::log10(upper) - std::log10(lower))/Math_t(band_count);
		Math_t const Q = (std::sqrt(lower * lower * std::pow(10, delta)))/(lower * std::pow(10,delta) - lower);

		m_Bands.reserve(band_count);

		for(uint32_t i = 0; i < band_count; ++i)
		{
			Math_t const l = i == 0 ? lower :(lower * std::pow(10, i*delta));
			Math_t const u = (i+1) == band_count ? upper : lower * std::pow(10, (i+1)*delta);

			m_Bands.push_back(
				BandPassPtr(
					new BandPass(std::sqrt(u*l), Q)
				)
			);
		}
	}

	void Equalizer::SetGain(uint32_t band, Math_t gain)
	{
		if(band > m_BandGains.size())
		{
			return;
		}

		m_BandGains[band] = gain;
	}

	Math_t Equalizer::GetGain(uint32_t band)
	{
		if(band > m_BandGains.size())
		{
			return 0;
		}

		return m_BandGains[band];
	}

	StereoData Equalizer::FilterSample(StereoData const & input)
	{
		StereoData output;

		for(uint32_t i = 0; i < uint32_t(m_Bands.size()); ++i)
		{
			StereoData band_out = m_Bands[i]->FilterSample(input);

			Left(output) += SampleType(m_BandGains[i] * Math_t(Left(band_out)));
			Right(output) += SampleType(m_BandGains[i] * Math_t(Right(band_out)));
		}

		return output;
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	Tools::MethodTable::MethodList_t Equalizer::CreateMethodList()
	{
		return {
			std::make_tuple(
				std::string("SetGain"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						auto t = *reinterpret_cast<std::tuple<uint32_t, Math_t>*>(p);
						SetGain(std::get<0>(t), std::get<1>(t));
					}
				)
			),
			std::make_tuple(
				std::string("GetGain"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						auto t = *reinterpret_cast<std::tuple<Math_t &, uint32_t>*>(p);
						std::get<0>(t) = GetGain(std::get<1>(t));
					}
				)
			)
		};
	}
} // namespace Modifier
} // namespace OCAE
