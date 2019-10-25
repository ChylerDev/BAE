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

namespace AudioEngine
{
namespace Modifier
{
	BandPass::BandPass(Math_t f, Math_t Q) :
		ModifierBase(false), m_CentralFrequency(f), m_Quality(Q),
		m_A0(), m_B1(), m_B2(),
		m_X1(), m_X2(), m_Y1(), m_Y2()
	{
		m_Table["SetFrequency"] = [this](void * freq){ SetFrequency(*reinterpret_cast<Math_t*>(freq)); };
		m_Table["SetQuality"] = [this](void * qual){ SetQuality(*reinterpret_cast<Math_t*>(qual)); };

		Reset();
	}

	BandPass::~BandPass()
	{
	}

	void BandPass::SetFrequency(Math_t f)
	{
		m_CentralFrequency = f;

		Reset();
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
			m_A0 * (Left(x) - Left(m_X2)) +
			m_B1 * Left(m_Y1) -
			m_B2 * Left(m_Y2)
		);
		Right(y) = SampleType(
			m_A0 * (Right(x) - Right(m_X2)) +
			m_B1 * Right(m_Y1) -
			m_B2 * Right(m_Y2)
		);

		m_Y2 = m_Y1;
		m_Y1 = y;
		m_X2 = m_X1;
		m_X1 = x;

		return y;
	}

	void BandPass::FilterBlock(StereoData * input, StereoData * output, uint64_t size)
	{
		static uint64_t i;

		for(i = 0; i < size; ++i)
		{
			Left(output[i]) += SampleType(
				m_A0 * (Left(input[i]) - Left(m_X2)) +
				m_B1 * Left(m_Y1) -
				m_B2 * Left(m_Y2)
			);
			Right(output[i]) += SampleType(
				m_A0 * (Right(input[i]) - Right(m_X2)) +
				m_B1 * Right(m_Y1) -
				m_B2 * Right(m_Y2)
			);

			m_Y2 = m_Y1;
			m_Y1 = output[i];
			m_X2 = m_X1;
			m_X1 = input[i];
		}
	}
} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{
	void BandPass::Reset()
	{
		#if 1
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

			thetaL = std::tan(PI*fL*INC_RATE);
			thetaH = std::tan(PI*fH*INC_RATE);

			aL = 1 / (1+thetaL);
			aH = 1 / (1+thetaH);

			bL = (1-thetaL) / (1+thetaL);
			bH = (1-thetaH) / (1+thetaH);

			m_A0 = (1-aL) * aH;

			m_B1 = bL + bH;
			m_B2 = bL * bH;
		#else
			Math_t theta = std::tan(PI*m_CentralFrequency*INC_RATE);
			Math_t thetasq = theta*theta;
			Math_t norm = 1.0/(m_Quality * thetasq + theta + m_Quality);

			m_A0 = theta*norm;
			m_B1 = 2 * m_Quality * (1-thetasq)*norm;
			m_B2 = (m_Quality*thetasq - theta+m_Quality)*norm;
		#endif
	}
} // namespace Modifier
} // namespace AudioEngine
