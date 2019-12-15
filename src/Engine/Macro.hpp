/*! ****************************************************************************
\file             Macro.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef _MACRO_HPP
#define _MACRO_HPP

// Public Macros                ////////////////////////////////////////////////

#ifndef SIZEOF_ARRAY
	#define SIZEOF_ARRAY(a) (sizeof(a)/sizeof(*a))
#endif // SIZEOF_ARRAY

#ifndef SAMPLE_RATE
		/// The sample rate OCAE runs at (probably 48kHz)
	#define SAMPLE_RATE 48000
#endif // SAMPLE_RATE
#ifndef INC_RATE
		/// Inverse of the sample rate
	#define INC_RATE (1.0/double(SAMPLE_RATE))
#endif // INC_RATE

#ifndef DEFAULT_GAIN
		/// Default amplification of the engine
	#define DEFAULT_GAIN Math_t(0.5)
#endif // DEFAULT_GAIN

#ifndef MAX_BUFFER
		/// Macro for the maximum buffer size to allow for high performant audio, which is currently defined as 1ms
	#define MAX_BUFFER (SAMPLE_RATE/1000)
#endif // MAX_BUFFER

#ifndef EPSILON
		/// Macro for the value at which we call the difference between two 64-bit floating point values effectively zero
	#define EPSILON (1.0/double(1 << 24))
#endif // EPSILON
#ifndef EPSILON_F
		/// Macro for the value at which we call the difference between two 32-bit floating point values effectively zero
	#define EPSILON_F (1.0f/float(1 << 16))
#endif // EPSILON_F

#include <cmath>

#ifndef PI
		/// It's uhh, it's Pi, the mathematical constant
	#define PI std::acos(-1.0)
#endif // PI
#ifndef PI2
		/// 2 * Pi, I hope I don't have to explain further
	#define PI2 (2*PI)
#endif // PI2
#ifndef LOG_10
		/// Logarithm of 10, for easy conversion of unknown bases to base 10
	#define LOG_10 std::log(10.0)
#endif // LOG_10
#ifndef SQRT_HALF
		/// sqrt(0.5) for easy use
	#define SQRT_HALF std::sqrt(0.5)
#endif // SQRT_HALF

#ifndef DB_TO_LINEAR
		/// Converts logarithmic decibels to linear gain
	#define DB_TO_LINEAR(dB) std::pow(10.0, dB/20.0)
#endif // DB_TO_LINEAR
#ifndef LINEAR_TO_DB
		/// Converts linear gain to logarithmic decibels
	#define LINEAR_TO_DB(g) (20.0*std::log(g)/LOG_10)
#endif // LINEAR_TO_DB

#ifndef MONO_TO_STEREO
		/// Converts monophonic audio sample to stereophonic
	#define MONO_TO_STEREO(x) StereoData(SampleType(Math_t(x)*SQRT_HALF),SampleType(Math_t(x)*SQRT_HALF))
#endif // MONO_TO_STEREO

#ifndef STEREO_TO_MONO
		/// Converts stereophonic audio sample to monophonic
	#define STEREO_TO_MONO(x) SampleType(Math_t(std::get<0>(x) + std::get<1>(x))/SQRT_HALF)
#endif // STEREO_TO_MONO

#include <type_traits>

#ifndef METHOD_RET_T
		/// Turns the given type into a reference
	#define METHOD_RET_T(t) std::add_lvalue_reference_t<std::remove_const_t<t>>
#endif // METHOD_RET_T
#ifndef METHOD_RET
		/// Casts the passed object to be a lvalue reference
	#define METHOD_RET(v) METHOD_RET_T(decltype(v))(v)
#endif // METHOD_RET
#ifndef METHOD_PARAM_T
		/// Turns the given type into a const reference
	#define METHOD_PARAM_T(t) std::add_lvalue_reference_t<t const>
#endif // METHOD_PARAM_T
#ifndef METHOD_PARAM
		/// Casts the passed object to the plain type
	#define METHOD_PARAM(v) METHOD_PARAM_T(decltype(v))(v)
#endif // METHOD_PARAM

#ifndef TYPEDEF_SHARED
	#include <memory>
		/// Creates an alias for std::shared_ptr instantiated with the given type
	#define TYPEDEF_SHARED(type) using type##Ptr = std::shared_ptr<type>
#endif // TYPEDEF_SHARED

#ifndef TO_STR
		/// Creates string from "p". E.g. TO_STR(HEAP_SIZE) creates the string "HEAP_SIZE"
	#define TO_STR(p) #p
#endif // TO_STR
#ifndef PRINT
		/// Creates string from what "p" defines. E.g. PRINT(HEAP_SIZE) creates the string "1024" if `HEAP_SIZE` is defined to `1024`
	#define PRINT(p) TO_STR(p)
#endif // PRINT

#ifndef DO_PRAGMA
	#if defined(_MSC_VER)
			/// Do platform-specific pragma command
		#define DO_PRAGMA(x) __pragma(x)
	#elif defined(__clang__) || defined(__GNUC__)
			/// Do platform-specific pragma command
		#define DO_PRAGMA(x) _Pragma(TO_STR(x))
	#else
			/// Do platform-specific pragma command
		#define DO_PRAGMA(x)
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // DO_PRAGMA

#ifndef TODO
	#if defined(_MSC_VER)
			/// Print the to-do message
		#define TODO(x) DO_PRAGMA(message(__FILE__ "(" PRINT(__LINE__) ") TODO - " TO_STR(x)))
	#elif defined(__clang__) || defined(__GNUC__)
			/// Print the to-do message
		#define TODO(x) DO_PRAGMA(message ("TODO - " TO_STR(x)))
	#else
			/// Print the to-do message
		#define TODO(x)
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif //TODO

#ifndef UNREFERENCED_PARAMETER
	#ifdef _MSC_VER
			/// Clears unused parameter warning
		#define UNREFERENCED_PARAMETER(P) (P)
	#else
			/// Clears unused parameter warning
		#define UNREFERENCED_PARAMETER(P) (void)(P)
	#endif // _WIN32 || __linux__
#endif // UNREFERENCED_PARAMETER

#ifndef PUSH_WARNINGS
	#if defined(_MSC_VER)
			/// Push warnings
		#define PUSH_WARNINGS() DO_PRAGMA(warning(push))
	#elif defined(__clang__)
			/// Push warnings
		#define PUSH_WARNINGS() DO_PRAGMA(clang diagnostic push)
	#elif defined(__GNUC__)
			/// Push warnings
		#define PUSH_WARNINGS() DO_PRAGMA(GCC diagnostic push)
	#else
			/// Push warnings
		#define PUSH_WARNINGS()
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // PUSH_WARNINGS

#ifndef MSVC_DISABLE_WARNING
	#if defined(_MSC_VER)
			/// Disable given VC++ warning
		#define MSVC_DISABLE_WARNING(x) DO_PRAGMA(warning(disable: x))
	#else
			/// Disable given VC++ warning
		#define MSVC_DISABLE_WARNING(x)
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // MSVC_DISABLE_WARNINGS
#ifndef CLANG_DISABLE_WARNING
	#if defined(__clang__)
			/// Disable given clang warning
		#define CLANG_DISABLE_WARNING(x) DO_PRAGMA(clang diagnostic ignored x)
	#else
			/// Disable given clang warning
		#define CLANG_DISABLE_WARNING(x)
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // CLANG_DISABLE_WARNING
#ifndef GCC_DISABLE_WARNING
	#if defined(__GNUC__)
			/// Disable given gcc warning
		#define GCC_DISABLE_WARNING(x) DO_PRAGMA(GCC diagnostic ignored x)
	#else
			/// Disable given gcc warning
		#define GCC_DISABLE_WARNING(x)
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // GCC_DISABLE_WARNING

#ifndef POP_WARNINGS
	#if defined(_MSC_VER)
			/// POP_WARNINGS
		#define POP_WARNINGS() DO_PRAGMA(warning(pop))
	#elif defined(__clang__)
			/// POP_WARNINGS
		#define POP_WARNINGS() DO_PRAGMA(clang diagnostic pop)
	#elif defined(__GNUC__)
			/// POP_WARNINGS
		#define POP_WARNINGS() DO_PRAGMA(GCC diagnostic pop)
	#else
			/// POP_WARNINGS
		#define POP_WARNINGS()
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // POP_WARNINGS

#endif // __MACRO_HPP
