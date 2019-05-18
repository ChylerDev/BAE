/*! ****************************************************************************
\file             LowPass.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "LowPass.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{

  LowPass::LowPass(float cutoff, float resonance) :
    Base(false),
    m_Cutoff(2*PI*cutoff), m_Resonance(resonance),
    m_Coefficients(), m_Outputs(), m_IsDirty(false)
  {
    Reset();
  }

  void LowPass::SetCutoff(float cutoff)
  {
    m_Cutoff = 2*PI*cutoff;
    m_IsDirty = true;
  }

  void LowPass::SetResonance(float resonance)
  {
    m_Resonance = resonance;
    m_IsDirty = true;
  }

  StereoData_t LowPass::FilterSample(StereoData_t const & input)
  {
    if(m_IsDirty)
    {
      Reset();
      m_IsDirty = false;
    }

    StereoData_t output(
      float(m_Coefficients[0] * std::get<0>(input) +
      m_Coefficients[1] * std::get<0>(m_Outputs[0]) +
      m_Coefficients[2] * std::get<0>(m_Outputs[1]) +
      m_Coefficients[3] * std::get<0>(m_Outputs[2])),

      float(m_Coefficients[0] * std::get<1>(input) +
      m_Coefficients[1] * std::get<1>(m_Outputs[0]) +
      m_Coefficients[2] * std::get<1>(m_Outputs[1]) +
      m_Coefficients[3] * std::get<1>(m_Outputs[2]))
    );

    //std::memmove(m_Outputs+1, m_Outputs, sizeof(*m_Outputs));
    for(uint32_t i = sizeof(m_Outputs) / sizeof(*m_Outputs) - 1; i > 0; --i)
    {
      m_Outputs[i] = m_Outputs[i - 1];
    }
    m_Outputs[0] = output;

    return output;
  }

  void LowPass::Reset()
  {
    static double angle, K, T, x, y, z, g;

    angle = (PI/6)*(4 - m_Resonance);
    K = 1 - 2*std::cos(angle);
    T = m_Cutoff * INC_RATE;
    x = K*T;
    y = K*T*T;
    z = T*T*T;
    g = 1 + x + y + z;

    m_Coefficients[3] =  1/g;
    m_Coefficients[0] =  m_Coefficients[3] * z;
    m_Coefficients[1] =  m_Coefficients[3] * (3 + 2*x + y);
    m_Coefficients[2] = -m_Coefficients[3] * (3 + x);
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
