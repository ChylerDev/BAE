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
		m_Inputs(std::get<0>(m_Zeros.back()), StereoData(0.f,0.f)),
		m_Outputs(std::get<0>(m_Poles.back()), StereoData(0.f,0.f)),
		m_MaxXSubscript(std::get<0>(m_Zeros.back())),
		m_MaxYSubscript(std::get<0>(m_Poles.back()))
	{
	}

	StereoData GenericFilter::FilterSample(StereoData const & input)
	{
		StereoData output(0.f,0.f);

		m_Inputs.push_front(input);
		m_Inputs.pop_back();

		for(auto const & x : m_Zeros)
		{
			Left(output) += SampleType(Left(m_Inputs[std::get<0>(x)]) * std::get<1>(x));
			Right(output) += SampleType(Right(m_Inputs[std::get<0>(x)]) * std::get<1>(x));
		}
		for(auto const & y : m_Poles)
		{
			Left(output) += SampleType(Left(m_Outputs[std::get<0>(y)]) * std::get<1>(y));
			Right(output) += SampleType(Right(m_Outputs[std::get<0>(y)]) * std::get<1>(y));
		}

		m_Outputs.push_front(output);
		m_Outputs.pop_back();

		return output;
	}

	void GenericFilter::FilterBlock(StereoData * input, StereoData * output, uint64_t size)
	{
		static uint64_t i;

		for(i = 0; i < size; ++i)
		{
			static StereoData out;

			m_Inputs.pop_back();
			m_Inputs.push_front(input[i]);

			for(auto const & x : m_Zeros)
			{
				Left(out) += SampleType(Left(m_Inputs[std::get<0>(x)]) * std::get<1>(x));
				Right(out) += SampleType(Right(m_Inputs[std::get<0>(x)]) * std::get<1>(x));
			}
			for(auto const & y : m_Poles)
			{
				Left(out) += SampleType(Left(m_Outputs[std::get<0>(y)]) * std::get<1>(y));
				Right(out) += SampleType(Right(m_Outputs[std::get<0>(y)]) * std::get<1>(y));
			}

			m_Outputs.pop_back();
			m_Outputs.push_front(out);

			Left(output[i]) += Left(out);
			Right(output[i]) += Right(out);
		}
	}
} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
