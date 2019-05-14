/*! ****************************************************************************
\file             Envelope.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Envelope.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{

  EnvelopeFollower::EnvelopeFollower(float fd, float fu) : Base(false)
  {
    // double theta_u = std::tan(PI * fu * INC_RATE);
    // double theta_d = std::tan(PI * fd * INC_RATE);

    // m_AU = theta_u / (1+theta_u);
    // m_BU = (1-theta_u) / (1+theta_u);
    // m_AD = theta_d / (1+theta_d);
    // m_BD = (1-theta_d) / (1+theta_d);

    double beta_u = SAMPLE_RATE / (SAMPLE_RATE + PI2 * fu);
    double beta_d = SAMPLE_RATE / (SAMPLE_RATE + PI2 * fd);

    m_AU = 1-beta_u;
    m_BU = beta_u;
    m_AD = 1-beta_d;
    m_BD = beta_d;
  }

  EnvelopeFollower::~EnvelopeFollower()
  {
  }

  StereoData_t EnvelopeFollower::FilterSample(StereoData_t const & x)
  {
    StereoData_t y;

    if(std::abs(std::get<0>(x)) > std::get<0>(m_Y1))
    {
      std::get<0>(y) = float(m_AU * std::abs(std::get<0>(x)) + m_BU*std::get<0>(m_Y1));
      // std::get<0>(y) = float(m_AU * (std::abs(std::get<0>(x))+std::abs(std::get<0>(m_X1))) + m_BU * std::get<0>(m_Y1));
    }
    else
    {
      std::get<0>(y) = float(m_AD * std::abs(std::get<0>(x)) + m_BD*std::get<0>(m_Y1));
      // std::get<0>(y) = float(m_AD * (std::abs(std::get<0>(x))+std::abs(std::get<0>(m_X1))) + m_BD * std::get<0>(m_Y1));
    }

    if(std::abs(std::get<1>(x)) > std::get<1>(m_Y1))
    {
      std::get<1>(y) = float(m_AU * std::abs(std::get<1>(x)) + m_BU*std::get<1>(m_Y1));
      // std::get<1>(y) = float(m_AU * (std::abs(std::get<1>(x))+std::abs(std::get<1>(m_X1))) + m_BU * std::get<1>(m_Y1));
    }
    else
    {
      std::get<1>(y) = float(m_AD * std::abs(std::get<1>(x)) + m_BD*std::get<1>(m_Y1));
      // std::get<1>(y) = float(m_AD * (std::abs(std::get<1>(x))+std::abs(std::get<1>(m_X1))) + m_BD * std::get<1>(m_Y1));
    }

    m_Y1 = y;
    m_X1 = x;

    return y;
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
