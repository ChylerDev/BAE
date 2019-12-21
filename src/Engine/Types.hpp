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

#include <functional>
#include <memory>
//#include <tuple>
#include <utility>

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Types                 ////////////////////////////////////////////////

namespace OCAE
{
		/// Define the type used for mathematics operations
	using Math_t = double;
		/// Define the type used for sample types
	using SampleType = float;
		/// Define the type used for stereo audio data
	using StereoData = std::pair<SampleType,SampleType>;
		/// Define the type used for stereo audio tracks
	using Track_t = std::vector<StereoData>;
} // namespace OCAE

#endif // __TYPES_HPP
