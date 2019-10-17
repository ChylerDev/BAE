/*! ****************************************************************************
\file             Util.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Util.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
  /*! **************************************************************************
  \brief
  
  *****************************************************************************/
  constexpr SampleType_t & Left(StereoData_t & s)
  {
	  return std::get<0>(s);
  }
  constexpr SampleType_t const & Left(StereoData_t const & s)
  {
	  return std::get<0>(s);
  }
  constexpr SampleType_t & Right(StereoData_t & s)
  {
	  return std::get<1>(s);
  }
  constexpr SampleType_t const & Right(StereoData_t const & s)
  {
	  return std::get<1>(s);
  }
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
