/*! ****************************************************************************
\file             BandPass.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cmath>

#include "BandPass.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	BandPass::BandPass(Math_t f, Math_t Q) : ModifierBase(),
		m_CentralFrequency(f), m_Quality(Q),
		m_A0(), m_B1(), m_B2(),
		m_X1(), m_X2(), m_Y1(), m_Y2()
	{
		RegisterMethods(CreateMethodList());
		Reset();
	}

	BandPass::~BandPass()
	{
	}

	Math_t BandPass::GetFrequency() const
	{
		return m_CentralFrequency;
	}

	void BandPass::SetFrequency(Math_t f)
	{
		m_CentralFrequency = f;

		Reset();
	}

	Math_t BandPass::GetQuality() const
	{
		return m_Quality;
	}

	void BandPass::SetQuality(Math_t Q)
	{
		m_Quality = Q;

		Reset();
	}

	StereoData BandPass::FilterSample(StereoData const & x)
	{
		StereoData y;

		Left(y) = SampleType(
			m_A0 * Math_t(Left(x) - Left(m_X2)) +
			m_B1 * Math_t(Left(m_Y1)) -
			m_B2 * Math_t(Left(m_Y2))
		);
		Right(y) = SampleType(
			m_A0 * Math_t(Right(x) - Right(m_X2)) +
			m_B1 * Math_t(Right(m_Y1)) -
			m_B2 * Math_t(Right(m_Y2))
		);

		m_Y2 = m_Y1;
		m_Y1 = y;
		m_X2 = m_X1;
		m_X1 = x;

		return y;
	}

	Tools::MethodTable::MethodList_t BandPass::CreateMethodList()
	{
		return {
			std::make_tuple(
				std::string("SetFrequency"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						SetFrequency(
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
				std::string("GetFrequency"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						std::get<0>(
							*reinterpret_cast<
								std::tuple<OCAE_METHOD_RET_T(Math_t)>*
							>(p)
						) = GetFrequency();
					}
				)
			),
			std::make_tuple(
				std::string("SetQuality"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						SetQuality(
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
				std::string("GetQuality"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						std::get<0>(
							*reinterpret_cast<
								std::tuple<OCAE_METHOD_RET_T(Math_t)>*
							>(p)
						) = GetQuality();
					}
				)
			)
		};
	}

	void BandPass::Reset()
	{
		Math_t fL, fH;

		Math_t a = 1;
		Math_t b = -m_CentralFrequency/m_Quality;
		Math_t c = -m_CentralFrequency*m_CentralFrequency;

		Math_t fL_q1 = (-b + std::sqrt(b*b - 4*a*c)) / (2*a);
		Math_t fL_q2 = (-b - std::sqrt(b*b - 4*a*c)) / (2*a);

		fL = (fL_q1 > 0 ? fL_q1 : fL_q2);
		fH = fL + b;

		////////

		Math_t thetaL, thetaH, aL, aH, bL, bH;

		thetaL = std::tan(OCAE_PI*fL*OCAE_INC_RATE);
		thetaH = std::tan(OCAE_PI*fH*OCAE_INC_RATE);

		aL = 1 / (1+thetaL);
		aH = 1 / (1+thetaH);

		bL = (1-thetaL) / (1+thetaL);
		bH = (1-thetaH) / (1+thetaH);

		m_A0 = (1-aL) * aH;

		m_B1 = bL + bH;
		m_B2 = bL * bH;
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
