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

namespace OCAE
{
namespace Modifier
{
	GenericFilter::GenericFilter(ZeroContainer const & zeros, PoleContainer const & poles) :
		ModifierBase(),
		m_Zeros(zeros), m_Poles(poles),
		m_Inputs(std::get<0>(m_Zeros.back())+1, StereoData()),
		m_Outputs(std::get<0>(m_Poles.back())+1, StereoData())
	{
	}

	StereoData GenericFilter::Process(StereoData const & input)
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

	Tools::MethodTable::MethodList_t GenericFilter::CreateMethodList()
	{
		return {};
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////