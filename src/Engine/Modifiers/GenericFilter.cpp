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
      std::get<0>(output) += std::get<0>(m_Inputs[std::get<0>(x)]) * std::get<1>(x);
      std::get<1>(output) += std::get<1>(m_Inputs[std::get<0>(x)]) * std::get<1>(x);
    }
    for(auto const & y : m_Poles)
    {
      std::get<0>(output) += std::get<0>(m_Outputs[std::get<0>(y)]) * std::get<1>(y);
      std::get<1>(output) += std::get<1>(m_Outputs[std::get<0>(y)]) * std::get<1>(y);
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
        std::get<0>(out) += std::get<0>(m_Inputs[std::get<0>(x)]) * std::get<1>(x);
        std::get<1>(out) += std::get<1>(m_Inputs[std::get<0>(x)]) * std::get<1>(x);
      }
      for(auto const & y : m_Poles)
      {
        std::get<0>(out) += std::get<0>(m_Outputs[std::get<0>(y)]) * std::get<1>(y);
        std::get<1>(out) += std::get<1>(m_Outputs[std::get<0>(y)]) * std::get<1>(y);
      }

      m_Outputs.pop_back();
      m_Outputs.push_front(out);

      std::get<0>(output[i]) += std::get<0>(out);
      std::get<1>(output[i]) += std::get<1>(out);
    }
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
