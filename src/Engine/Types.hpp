/*! ****************************************************************************
\file             Types.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __TYPES_HPP
#define __TYPES_HPP

// Include Files                ////////////////////////////////////////////////

#include <cstdint>

#include <functional>
#include <memory>
#include <tuple>

#define FIXED_POINT_RAW_ACCESS
#include <FixedPoint/FixedPoint.hpp>

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Types                 ////////////////////////////////////////////////

namespace AudioEngine
{

  using Math_t = double;//FixedPoint::FixedPoint<15,16>;
  using SampleType_t = float;//FixedPoint::Audio16Bit_t;
  using StereoData_t = std::tuple<SampleType_t,SampleType_t>;
  using pStereoData_t = std::shared_ptr<StereoData_t>;
  using aStereoData_t = std::shared_ptr<StereoData_t[]>;

  using Track_t = std::vector<StereoData_t>;

  using AudioCallback_t = std::function<StereoData_t(void)>;

  using Void_fn = std::function<void(void*)>;
  using MethodTable_t = std::unordered_map<std::string, Void_fn>;

} // namespace AudioEngine

#endif  // __TYPES_HPP
