/*! ****************************************************************************
\file             GenericFilter.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "GenericFilter.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{
	GenericFilter::GenericFilter(ZeroContainer const & zeros, PoleContainer const & poles) :
		ModifierBase(false),
		m_Zeros(zeros), m_Poles(poles),
		m_Inputs(std::get<0>(m_Zeros.back()), StereoData()),
		m_Outputs(std::get<0>(m_Poles.back()), StereoData()),
		m_MaxXSubscript(std::get<0>(m_Zeros.back())),
		m_MaxYSubscript(std::get<0>(m_Poles.back()))
	{
	}

	StereoData GenericFilter::FilterSample(StereoData const & input)
	{
		StereoData output;

		m_Inputs.push_front(input);
		m_Inputs.pop_back();

		for(auto const & x : m_Zeros)
		{
			 Left(output) += SampleType(Math_t( Left(m_Inputs[std::get<0>(x)])) * std::get<1>(x));
			Right(output) += SampleType(Math_t(Right(m_Inputs[std::get<0>(x)])) * std::get<1>(x));
		}
		for(auto const & y : m_Poles)
		{
			 Left(output) += SampleType(Math_t( Left(m_Outputs[std::get<0>(y)])) * std::get<1>(y));
			Right(output) += SampleType(Math_t(Right(m_Outputs[std::get<0>(y)])) * std::get<1>(y));
		}

		m_Outputs.push_front(output);
		m_Outputs.pop_back();

		return output;
	}
} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
