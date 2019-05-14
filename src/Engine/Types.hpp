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
#include <tuple>

// Public Macros                ////////////////////////////////////////////////

// Public Types                 ////////////////////////////////////////////////

namespace AudioEngine
{

    // StereoData_t[0] = left; StereoData_t[1] = right;
  using StereoData_t = std::tuple<float,float>;
  using AudioCallback_t = std::function<StereoData_t(void)>;

  using Track_t = std::vector<StereoData_t>;

} // namespace AudioEngine

#endif  // __TYPES_HPP
