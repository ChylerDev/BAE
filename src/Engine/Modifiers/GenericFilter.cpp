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
    m_Zeros(zeros), m_Poles(poles),
    m_MaxXSubscript(std::get<0>(m_Zeros.back())),
    m_MaxYSubscript(std::get<0>(m_Poles.back()))
  {
      // Fill input container with 0-value samples
    for(uint32_t i = 0; i <= m_MaxXSubscript; ++i)
    {
      m_Inputs.push_back(StereoData_t(0.f,0.f));
    }
      // Fill output container with 0-value samples
    for(uint32_t i = 0; i <= m_MaxYSubscript; ++i)
    {
      m_Outputs.push_back(StereoData_t(0.f,0.f));
    }
  }

  StereoData_t GenericFilter::FilterSample(StereoData_t input)
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

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
