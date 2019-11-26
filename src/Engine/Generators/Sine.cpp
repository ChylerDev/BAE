/*! ****************************************************************************
\file             Sine.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cmath>

#include "Sine.hpp"

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
	Sine::Sine(Math_t f) : GeneratorBase(),
		irate(INC_RATE*double(f)),
		y1(SampleType(std::sin(double(PI2 * irate)))), y2(), beta()
	{
		RegisterMethods(CreateMethodList());
		Reset();
	}

	StereoData Sine::SendSample(void)
	{
		SampleType y = SampleType(beta * Math_t(y1) - Math_t(y2));

		y2 = y1;
		y1 = y;

		return MONO_TO_STEREO(y);
	}

	void Sine::SetFrequency(Math_t f)
	{
		irate = INC_RATE * double(f);
		Reset();
	}

	Math_t Sine::GetFrequency() const
	{
		return Math_t(irate * SAMPLE_RATE);
	}

	Tools::MethodTable::MethodList_t Sine::CreateMethodList()
	{
		return {
			std::make_tuple(
				std::string("SetFrequency"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						SetFrequency(
							std::get<0>(
								*reinterpret_cast<
									std::tuple<METHOD_PARAM_T(Math_t)>*
								>(p)
							)
						);
					}
				)
			),
			std::make_tuple(
				std::string("GetFrequency"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						std::get<0>(
							*reinterpret_cast<
								std::tuple<METHOD_RET_T(Math_t)>*
							>(p)
						) = GetFrequency();
					}
				)
			)
		};
	}

	void Sine::Reset()
	{
		beta = Math_t(2 * std::cos(double(2*PI * irate)));
	}
} // namespace Generator
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
