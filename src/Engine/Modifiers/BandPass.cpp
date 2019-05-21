/*! ****************************************************************************
\file             BandPass.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
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
    Base(false), m_CentralFrequency(f), m_Quality(Q)
  {
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

  StereoData_t BandPass::FilterSample(StereoData_t const & x)
  {
    StereoData_t y;

    std::get<0>(y) = SampleType_t(
      m_A0 * (std::get<0>(x) - std::get<0>(m_X2)) +
      m_B1 * std::get<0>(m_Y1) -
      m_B2 * std::get<0>(m_Y2)
    );
    std::get<1>(y) = SampleType_t(
      m_A0 * (std::get<1>(x) - std::get<1>(m_X2)) +
      m_B1 * std::get<1>(m_Y1) -
      m_B2 * std::get<1>(m_Y2)
    );

    m_Y2 = m_Y1;
    m_Y1 = y;
    m_X2 = m_X1;
    m_X1 = x;

    return y;
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{

  TODO("Once FixedPoint supports operator/, change doubles to Math_t/SampleType_t")
  void BandPass::Reset()
  {
    #if 1
      double fL, fH;

      double a = 1;
      double b = double(-m_CentralFrequency)/double(m_Quality);
      double c = double(-m_CentralFrequency*m_CentralFrequency);

      double fL_q1 = (-b + std::sqrt(b*b - 4*a*c)) / (2*a);
      double fL_q2 = (-b - std::sqrt(b*b - 4*a*c)) / (2*a);

      fL = (fL_q1 > 0 ? fL_q1 : fL_q2);
      fH = fL + b;

      ////////

      double thetaL, thetaH, aL, aH, bL, bH;

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
      double theta = std::tan(PI*m_CentralFrequency*INC_RATE);
      double thetasq = theta*theta;
      double norm = 1.0/(m_Quality * thetasq + theta + m_Quality);

      m_A0 = theta*norm;
      m_B1 = 2 * m_Quality * (1-thetasq)*norm;
      m_B2 = (m_Quality*thetasq - theta+m_Quality)*norm;
    #endif
  }

} // namespace Modifier
} // namespace AudioEngine
