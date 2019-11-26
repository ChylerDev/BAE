/*! ****************************************************************************
\file             Envelope.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Envelope.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	EnvelopeFollower::EnvelopeFollower(Math_t fd, Math_t fu) : ModifierBase(),
		m_AU(), m_BU(), m_AD(), m_BD(), m_X1(), m_Y1()
	{
		RegisterMethods(CreateMethodList());

		double theta_u = std::tan(PI * fu * INC_RATE);
		double theta_d = std::tan(PI * fd * INC_RATE);

		m_AU = theta_u / (1+theta_u);
		m_BU = (1-theta_u) / (1+theta_u);
		m_AD = theta_d / (1+theta_d);
		m_BD = (1-theta_d) / (1+theta_d);
	}

	EnvelopeFollower::~EnvelopeFollower()
	{
	}

	StereoData EnvelopeFollower::FilterSample(StereoData const & x)
	{
		StereoData y;

		if(+(Left(x)) > Left(m_Y1))
		{
			Left(y) = SampleType(m_AU * +Math_t(Left(x)) + m_BU*Math_t(Left(m_Y1)));
		}
		else
		{
			Left(y) = SampleType(m_AD * +Math_t(Left(x)) + m_BD*Math_t(Left(m_Y1)));
		}

		if(+(Right(x)) > Right(m_Y1))
		{
			Right(y) = SampleType(m_AU * +Math_t(Right(x)) + m_BU*Math_t(Right(m_Y1)));
		}
		else
		{
			Right(y) = SampleType(m_AD * +Math_t(Right(x)) + m_BD*Math_t(Right(m_Y1)));
		}

		m_Y1 = y;
		m_X1 = x;

		return y;
	}

	Tools::MethodTable::MethodList_t EnvelopeFollower::CreateMethodList()
	{
		return {};
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
