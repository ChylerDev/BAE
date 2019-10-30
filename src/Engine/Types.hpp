/*! ****************************************************************************
\file             Types.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __TYPES_HPP
#define __TYPES_HPP

// Include Files                ////////////////////////////////////////////////

#include <cstdint>

#include <memory>
#include <tuple>

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Types                 ////////////////////////////////////////////////

namespace OCAE
{
	using Math_t = double;
	using SampleType = float;
	TYPEDEF_SHARED(SampleType);
	using StereoData = std::tuple<SampleType,SampleType>;
	TYPEDEF_SHARED(StereoData);
	using Track_t = std::vector<StereoData>;
} // namespace OCAE

#endif  // __TYPES_HPP
