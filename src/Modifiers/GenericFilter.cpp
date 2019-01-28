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

namespace Modifiers
{

  GenericFilter::GenericFilter(ZeroContainer const & zeros, PoleContainer const & poles) :
    m_Zeros(zeros), m_Poles(poles),
    m_MaxXSubscript(std::get<0>(m_Zeros.back())),
    m_MaxYSubscript(std::get<0>(m_Poles.back()))
  {
  }

  StereoData_t GenericFilter::FilterSample(StereoData_t x)
  {
    
  }

} // namespace Modifiers

// Private Functions                      //////////////////////////////////////
