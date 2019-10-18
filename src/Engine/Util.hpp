/*! ****************************************************************************
\file             Util.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __UTIL_HPP
#define __UTIL_HPP

// Include Files                ////////////////////////////////////////////////

#include "Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

// Public Functions             ////////////////////////////////////////////////

namespace AudioEngine
{
  /*! **************************************************************************
  \brief
  
  *****************************************************************************/
	static constexpr SampleType_t& Left(StereoData_t& s)
	{
		return std::get<0>(s);
	}
	static constexpr SampleType_t const& Left(StereoData_t const& s)
	{
		return std::get<0>(s);
	}
	static constexpr SampleType_t& Right(StereoData_t& s)
	{
		return std::get<1>(s);
	}
	static constexpr SampleType_t const& Right(StereoData_t const& s)
	{
		return std::get<1>(s);
	}
} // namespace AudioEngine

#endif // __UTIL_HPP
