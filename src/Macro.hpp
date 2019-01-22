/*! ****************************************************************************
\file             Macro.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef _MACRO_HPP
#define _MACRO_HPP

// Public Macros                ////////////////////////////////////////////////

#ifndef SAMPLE_RATE
  #define SAMPLE_RATE 48'000
#endif
#ifndef INC_RATE_F
  #define INC_RATE_F (1.f/float(SAMPLE_RATE))
#endif
#ifndef INC_RATE
  #define INC_RATE (1.0/double(SAMPLE_RATE))
#endif

#include <cmath>

#ifndef PI_INC
  static double const PI        = std::acos(-1.0);
  static double const PI2       = 2*PI;
  static float  const PI_F      = std::acos(-1.f);
  static double const LOG_10    = std::log(10.0f);
  static double const SQRT_HALF = std::sqrt(0.5);

  #define PI_INC
#endif

#ifndef DB_TO_LINEAR
  #define DB_TO_LINEAR(dB) std::pow(10.0, dB/20.0)
#endif
#ifndef LINEAR_TO_DB
  #define LINEAR_TO_DB(g) (20.0*std::log(g)/LOG_10)
#endif

#ifndef MONO_TO_STEREO
  #define MONO_TO_STEREO(x) StereoData_t(float(x*SQRT_HALF),float(x*SQRT_HALF))
#endif

#ifndef STEREO_TO_MONO
  #define STEREO_TO_MONO(x) ((std::get<0>(x) + std::get<1>(x))*0.5)
#endif

#ifndef TO_STR
  /// Creates string from "p". E.g. TO_STR(HEAP_SIZE) creates the string "HEAP_SIZE"
 #define TO_STR(p) #p
#endif  // TO_STR
#ifndef PRINT
  /// Creates string from what "p" defines. E.g. PRINT(HEAP_SIZE) creates the string "1024" if #define HEAP_SIZE 1024
 #define PRINT(p) TO_STR(p)
#endif // PRINT

#ifndef TODO
  #if defined(_MSC_VER)
      /// Print the message
    #define TODO(x) __pragma(message(__FILE__ "(" PRINT(__LINE__) ") TODO - " TO_STR(x)))
  #elif defined(__clang__) || defined(__GNUC__)
      /// Print the message
    #define DO_PRAGMA(x) _Pragma (TO_STR(x))
      /// Primes the message and calls the pragma function
    #define TODO(x) DO_PRAGMA(message ("TODO - " TO_STR(x)))
  #else
      /// If unkown compiler, don't assume any knowledge of pragma/messages
    #define TODO(x)
 #endif // _MSC_VER || (__clang__ || __GNUC__)
#endif  //TODO

#ifndef UNREFERENCED_PARAMETER
  #ifdef _MSC_VER
      /// Clears unused parameter warning in MSVC
    #define UNREFERENCED_PARAMETER(P) (P)
  #else
      /// Clears unused parameter warning
    #define UNREFERENCED_PARAMETER(P) (void)(P)
  #endif // _WIN32 || __linux__
#endif // UNREFERENCED_PARAMETER

#ifndef EPSILON
  /// Macro for the value at which we call the difference between two values effectively zero
 #define EPSILON (1.0/(1 << 16))
#endif  // EPSILON
#ifndef EPSILON_F
  /// Macro for the value at which we call the difference between two values effectively zero
 #define EPSILON_F (1.0f/(1 << 16))
#endif  // EPSILON_F

#endif  // __MACRO_HPP
