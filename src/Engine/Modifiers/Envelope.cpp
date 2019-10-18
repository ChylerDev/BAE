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

  EnvelopeFollower::EnvelopeFollower(Math_t fd, Math_t fu) : Base(false),
    m_AU(), m_BU(), m_AD(), m_BD(), m_X1(), m_Y1()
  {
    double theta_u = std::tan(PI * fu * INC_RATE);
    double theta_d = std::tan(PI * fd * INC_RATE);

    m_AU = theta_u / (1+theta_u);
    m_BU = (1-theta_u) / (1+theta_u);
    m_AD = theta_d / (1+theta_d);
    m_BD = (1-theta_d) / (1+theta_d);

    // double beta_u = SAMPLE_RATE / double(SAMPLE_RATE + PI2 * fu);
    // double beta_d = SAMPLE_RATE / double(SAMPLE_RATE + PI2 * fd);

    // m_AU = 1-beta_u;
    // m_BU = beta_u;
    // m_AD = 1-beta_d;
    // m_BD = beta_d;
  }

  EnvelopeFollower::~EnvelopeFollower()
  {
  }

  StereoData_t EnvelopeFollower::FilterSample(StereoData_t const & x)
  {
    StereoData_t y;

    if(+(Left(x)) > Left(m_Y1))
    {
      Left(y) = float(m_AU * +(Left(x)) + m_BU*Left(m_Y1));
      // Left(y) = float(m_AU * (+(Left(x)) + +(Left(m_X1))) + m_BU * Left(m_Y1));
    }
    else
    {
      Left(y) = float(m_AD * +(Left(x)) + m_BD*Left(m_Y1));
      // Left(y) = float(m_AD * (+(Left(x)) + +(Left(m_X1))) + m_BD * Left(m_Y1));
    }

    if(+(Right(x)) > Right(m_Y1))
    {
      Right(y) = float(m_AU * +(Right(x)) + m_BU*Right(m_Y1));
      // Right(y) = float(m_AU * (+(Right(x)) + +(Right(m_X1))) + m_BU * Right(m_Y1));
    }
    else
    {
      Right(y) = float(m_AD * +(Right(x)) + m_BD*Right(m_Y1));
      // Right(y) = float(m_AD * (+(Right(x)) + +(Right(m_X1))) + m_BD * Right(m_Y1));
    }

    m_Y1 = y;
    m_X1 = x;

    return y;
  }

  void EnvelopeFollower::FilterBlock(StereoData_t * input, StereoData_t * output, uint64_t size)
  {
    static uint64_t i;

    for(i = 0; i < size; ++i)
    {
      static StereoData_t sample;

      if(+(Left(input[i])) > Left(m_Y1))
        Left(sample) = float(m_AU * (+(Left(input[i])) +
                                    +(Left(m_X1))) + m_BU * Left(m_Y1));
      else
        Left(sample) = float(m_AD * (+(Left(input[i])) +
                                    +(Left(m_X1))) + m_BD * Left(m_Y1));

      if(+(Right(input[i])) > Right(m_Y1))
        Right(sample) = float(m_AU * (+(Right(input[i])) +
                                    +(Right(m_X1))) + m_BU * Right(m_Y1));
      else
        Right(sample) = float(m_AD * (+(Right(input[i])) +
                                    +(Right(m_X1))) + m_BD * Right(m_Y1));

      m_Y1 = sample;
      m_X1 = input[i];

      Left(output[i]) += Left(sample);
      Right(output[i]) += Right(sample);
    }
  }

} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
