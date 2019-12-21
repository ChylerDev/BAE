/*! ****************************************************************************
\file             Echo.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Echo.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	Echo::Echo(uint64_t sample_delay, Math_t decay_ratio) : ModifierBase(),
		m_Echo(sample_delay, StereoData()), m_Ratio(decay_ratio)
	{
		RegisterMethods(CreateMethodList());
	}

	StereoData Echo::FilterSample(StereoData const & dry)
	{
		StereoData wet = m_Echo.front();
		m_Echo.pop_front();

		StereoData out = StereoData(
			SampleType(Math_t( Left(wet)) * m_Ratio + Math_t( Left(dry))),
			SampleType(Math_t(Right(wet)) * m_Ratio + Math_t(Right(dry)))
		);

		m_Echo.push_back(out);

		return out;
	}

	void Echo::SetDecayRatio(Math_t ratio)
	{
		m_Ratio = ratio;
	}

	Math_t Echo::GetDecayRatio() const
	{
		return m_Ratio;
	}

	Tools::MethodTable::MethodList_t Echo::CreateMethodList()
	{
		return {
			std::make_tuple(
				std::string("SetDecayRatio"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						SetDecayRatio(
							std::get<0>(
								*reinterpret_cast<
									std::tuple<OCAE_METHOD_PARAM_T(Math_t)>*
								>(p)
							)
						);
					}
				)
			),
			std::make_tuple(
				std::string("GetDecayRatio"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						std::get<0>(
							*reinterpret_cast<
								std::tuple<OCAE_METHOD_RET_T(Math_t)>*
							>(p)
						) = GetDecayRatio();
					}
				)
			)
		};
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
