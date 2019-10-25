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
	constexpr SampleType & Left(StereoData& s)
	{
		return std::get<0>(s);
	}

	constexpr SampleType const& Left(StereoData const& s)
	{
		return std::get<0>(s);
	}

	constexpr SampleType & Right(StereoData& s)
	{
		return std::get<1>(s);
	}

	constexpr SampleType const& Right(StereoData const& s)
	{
		return std::get<1>(s);
	}
} // namespace AudioEngine

#endif // __UTIL_HPP
