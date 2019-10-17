/*! ****************************************************************************
\file             GenericFilter.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
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
    Base(false),
    m_Zeros(zeros), m_Poles(poles),
    m_Inputs(std::get<0>(m_Zeros.back()), StereoData_t(0.f,0.f)),
    m_Outputs(std::get<0>(m_Poles.back()), StereoData_t(0.f,0.f)),
    m_MaxXSubscript(std::get<0>(m_Zeros.back())),
    m_MaxYSubscript(std::get<0>(m_Poles.back()))
  {
  }

  StereoData_t GenericFilter::FilterSample(StereoData_t const & input)
  {
    StereoData_t output(0.f,0.f);

    m_Inputs.push_front(input);
    m_Inputs.pop_back();

    for(auto const & x : m_Zeros)
    {
      Left(output) += Left(m_Inputs[std::get<0>(x)]) * Right(x);
      Right(output) += Right(m_Inputs[std::get<0>(x)]) * Right(x);
    }
    for(auto const & y : m_Poles)
    {
      Left(output) += std::get<0>(m_Outputs[std::get<0>(y)]) * Right(y);
      Right(output) += Right(m_Outputs[std::get<0>(y)]) * Right(y);
    }

    m_Outputs.push_front(output);
    m_Outputs.pop_back();

    return output;
  }

  void GenericFilter::FilterBlock(StereoData_t * input, StereoData_t * output, uint64_t size)
  {
    static uint64_t i;

    for(i = 0; i < size; ++i)
    {
      static StereoData_t out;

      m_Inputs.pop_back();
      m_Inputs.push_front(input[i]);

      for(auto const & x : m_Zeros)
      {
        Left(out) += Left(m_Inputs[Left(x)]) * Right(x);
        Right(out) += Right(m_Inputs[Left(x)]) * Right(x);
      }
      for(auto const & y : m_Poles)
      {
        Left(out) += Left(m_Outputs[Left(y)]) * Right(y);
        Right(out) += Right(m_Outputs[Left(y)]) * Right(y);
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
