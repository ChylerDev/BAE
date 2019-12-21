/*! ****************************************************************************
\file             Macro.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __OCAE_MACRO_HPP
#define __OCAE_MACRO_HPP

// Public Macros                ////////////////////////////////////////////////

#ifndef OCAE_SIZEOF_ARRAY
	#define OCAE_SIZEOF_ARRAY(a) (sizeof(a)/sizeof(*a))
#endif // OCAE_SIZEOF_ARRAY

#ifndef OCAE_SAMPLE_RATE
		/// The sample rate OCAE runs at (probably 48kHz)
	#define OCAE_SAMPLE_RATE 48000
#endif // OCAE_SAMPLE_RATE
#ifndef OCAE_INC_RATE
		/// Inverse of the sample rate
	#define OCAE_INC_RATE (1.0/double(OCAE_SAMPLE_RATE))
#endif // OCAE_INC_RATE

#ifndef OCAE_DEFAULT_GAIN
		/// Default amplification of the engine
	#define OCAE_DEFAULT_GAIN Math_t(0.5)
#endif // OCAE_DEFAULT_GAIN

#ifndef OCAE_MAX_BUFFER
		/// Macro for the maximum buffer size to allow for high performant audio, which is currently defined as 1ms
	#define OCAE_MAX_BUFFER (OCAE_SAMPLE_RATE/1000)
#endif // OCAE_MAX_BUFFER

#ifndef OCAE_EPSILON
		/// Macro for the value at which we call the difference between two 64-bit floating point values effectively zero
	#define OCAE_EPSILON (1.0/double(1 << 24))
#endif // OCAE_EPSILON
#ifndef OCAE_EPSILON_F
		/// Macro for the value at which we call the difference between two 32-bit floating point values effectively zero
	#define OCAE_EPSILON_F (1.0f/float(1 << 16))
#endif // OCAE_EPSILON_F

#include <cmath>

#ifndef OCAE_PI
		/// It's uhh, it's Pi, the mathematical constant
	#define OCAE_PI std::acos(-1.0)
#endif // OCAE_PI
#ifndef OCAE_PI2
		/// 2 * Pi, I hope I don't have to explain further
	#define OCAE_PI2 (2*OCAE_PI)
#endif // OCAE_PI2
#ifndef OCAE_LOG_10
		/// Logarithm of 10, for easy conversion of unknown bases to base 10
	#define OCAE_LOG_10 std::log(10.0)
#endif // OCAE_LOG_10
#ifndef OCAE_SQRT_HALF
		/// sqrt(0.5) for easy use
	#define OCAE_SQRT_HALF std::sqrt(0.5)
#endif // OCAE_SQRT_HALF

#ifndef OCAE_DB_TO_LINEAR
		/// Converts logarithmic decibels to linear gain
	#define OCAE_DB_TO_LINEAR(dB) std::pow(10.0, dB/20.0)
#endif // OCAE_DB_TO_LINEAR
#ifndef OCAE_LINEAR_TO_DB
		/// Converts linear gain to logarithmic decibels
	#define OCAE_LINEAR_TO_DB(g) (20.0*std::log(g)/OCAE_LOG_10)
#endif // OCAE_LINEAR_TO_DB

#ifndef OCAE_MONO_TO_STEREO
		/// Converts monophonic audio sample to stereophonic
	#define OCAE_MONO_TO_STEREO(x) StereoData(SampleType(Math_t(x)*OCAE_SQRT_HALF),SampleType(Math_t(x)*OCAE_SQRT_HALF))
#endif // OCAE_MONO_TO_STEREO

#ifndef OCAE_STEREO_TO_MONO
		/// Converts stereophonic audio sample to monophonic
	#define OCAE_STEREO_TO_MONO(x) SampleType(Math_t(std::get<0>(x) + std::get<1>(x))/OCAE_SQRT_HALF)
#endif // OCAE_STEREO_TO_MONO

#include <type_traits>

#ifndef OCAE_METHOD_RET_T
		/// Turns the given type into a reference
	#define OCAE_METHOD_RET_T(t) std::add_lvalue_reference_t<std::remove_const_t<t>>
#endif // OCAE_METHOD_RET_T
#ifndef OCAE_METHOD_RET
		/// Casts the passed object to be a lvalue reference
	#define OCAE_METHOD_RET(v) OCAE_METHOD_RET_T(decltype(v))(v)
#endif //OCAE_ OCAE_METHOD_RET
#ifndef OCAE_METHOD_PARAM_T
		/// Turns the given type into a const reference
	#define OCAE_METHOD_PARAM_T(t) std::add_lvalue_reference_t<t const>
#endif // OCAE_METHOD_PARAM_T
#ifndef OCAE_METHOD_PARAM
		/// Casts the passed object to the plain type
	#define OCAE_METHOD_PARAM(v) OCAE_METHOD_PARAM_T(decltype(v))(v)
#endif // OCAE_METHOD_PARAM

#ifndef OCAE_TYPEDEF_SHARED
	#include <memory>
		/// Creates an alias for std::shared_ptr instantiated with the given type
	#define OCAE_TYPEDEF_SHARED(type) using type##Ptr = std::shared_ptr<type>
#endif // OCAE_TYPEDEF_SHARED

#ifndef OCAE_TO_STR
		/// Creates string from "p". E.g. OCAE_TO_STR(HEAP_SIZE) creates the string "HEAP_SIZE"
	#define OCAE_TO_STR(p) #p
#endif // OCAE_TO_STR
#ifndef OCAE_PRINT
		/// Creates string from what "p" defines. E.g. PRINT(HEAP_SIZE) creates the string "1024" if `HEAP_SIZE` is defined to `1024`
	#define OCAE_PRINT(p) OCAE_TO_STR(p)
#endif // OCAE_PRINT

#ifndef OCAE_DO_PRAGMA
	#if defined(_MSC_VER)
			/// Do platform-specific pragma command
		#define OCAE_DO_PRAGMA(x) __pragma(x)
	#elif defined(__clang__) || defined(__GNUC__)
			/// Do platform-specific pragma command
		#define OCAE_DO_PRAGMA(x) _Pragma(OCAE_TO_STR(x))
	#else
			/// Do platform-specific pragma command
		#define OCAE_DO_PRAGMA(x)
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // OCAE_DO_PRAGMA

#ifndef OCAE_TODO
	#if defined(_MSC_VER)
			/// Print the to-do message
		#define OCAE_TODO(x) OCAE_DO_PRAGMA(message(__FILE__ "(" OCAE_PRINT(__LINE__) ") OCAE_TODO - " OCAE_TO_STR(x)))
	#elif defined(__clang__) || defined(__GNUC__)
			/// Print the to-do message
		#define OCAE_TODO(x) OCAE_DO_PRAGMA(message ("OCAE_TODO - " OCAE_TO_STR(x)))
	#else
			/// Print the to-do message
		#define OCAE_TODO(x)
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // OCAE_TODO

#ifndef OCAE_UNREFERENCED_PARAMETER
	#ifdef _MSC_VER
			/// Clears unused parameter warning
		#define OCAE_UNREFERENCED_PARAMETER(P) (P)
	#else
			/// Clears unused parameter warning
		#define OCAE_UNREFERENCED_PARAMETER(P) (void)(P)
	#endif // _WIN32 || __linux__
#endif // OCAE_UNREFERENCED_PARAMETER

#ifndef OCAE_PUSH_WARNINGS
	#if defined(_MSC_VER)
			/// Push warnings
		#define OCAE_PUSH_WARNINGS() OCAE_DO_PRAGMA(warning(push))
	#elif defined(__clang__)
			/// Push warnings
		#define OCAE_PUSH_WARNINGS() OCAE_DO_PRAGMA(clang diagnostic push)
	#elif defined(__GNUC__)
			/// Push warnings
		#define OCAE_PUSH_WARNINGS() OCAE_DO_PRAGMA(GCC diagnostic push)
	#else
			/// Push warnings
		#define OCAE_PUSH_WARNINGS()
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // OCAE_PUSH_WARNINGS

#ifndef OCAE_MSVC_DISABLE_WARNING
	#if defined(_MSC_VER)
			/// Disable given VC++ warning
		#define OCAE_MSVC_DISABLE_WARNING(x) OCAE_DO_PRAGMA(warning(disable: x))
	#else
			/// Disable given VC++ warning
		#define OCAE_MSVC_DISABLE_WARNING(x)
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // OCAE_MSVC_DISABLE_WARNINGS
#ifndef OCAE_CLANG_DISABLE_WARNING
	#if defined(__clang__)
			/// Disable given clang warning
		#define OCAE_CLANG_DISABLE_WARNING(x) OCAE_DO_PRAGMA(clang diagnostic ignored x)
	#else
			/// Disable given clang warning
		#define OCAE_CLANG_DISABLE_WARNING(x)
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // OCAE_CLANG_DISABLE_WARNING
#ifndef OCAE_GCC_DISABLE_WARNING
	#if defined(__GNUC__)
			/// Disable given gcc warning
		#define OCAE_GCC_DISABLE_WARNING(x) OCAE_DO_PRAGMA(GCC diagnostic ignored x)
	#else
			/// Disable given gcc warning
		#define OCAE_GCC_DISABLE_WARNING(x)
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // OCAE_GCC_DISABLE_WARNING

#ifndef OCAE_POP_WARNINGS
	#if defined(_MSC_VER)
			/// POP_WARNINGS
		#define OCAE_POP_WARNINGS() OCAE_DO_PRAGMA(warning(pop))
	#elif defined(__clang__)
			/// POP_WARNINGS
		#define OCAE_POP_WARNINGS() OCAE_DO_PRAGMA(clang diagnostic pop)
	#elif defined(__GNUC__)
			/// POP_WARNINGS
		#define OCAE_POP_WARNINGS() OCAE_DO_PRAGMA(GCC diagnostic pop)
	#else
			/// POP_WARNINGS
		#define OCAE_POP_WARNINGS()
	#endif // _MSC_VER || __clang__ || __GNUC__
#endif // OCAE_POP_WARNINGS

#endif // __OCAE_MACRO_HPP
