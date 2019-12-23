/*! ****************************************************************************
\file             Util.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __OCAE_UTIL_HPP
#define __OCAE_UTIL_HPP

// Include Files                ////////////////////////////////////////////////

#include "Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

// Public Functions             ////////////////////////////////////////////////

namespace OCAE
{
	/*! ************************************************************************
	\brief
		Returns the left audio sample from a stereo data pair.

	\param s
		The stereo audio sample.

	\return
		The left audio sample.
	***************************************************************************/
	constexpr SampleType & Left(StereoData& s)
	{
		return s.first;//std::get<0>(s);
	}

	/*! ************************************************************************
	\brief
		Returns the left audio sample from a stereo data pair.

	\param s
		The stereo audio sample.

	\return
		The left audio sample.
	***************************************************************************/
	constexpr SampleType const & Left(StereoData const & s)
	{
		return s.first;//std::get<0>(s);
	}

	/*! ************************************************************************
	\brief
		Returns the right audio sample from a stereo data pair.

	\param s
		The stereo audio sample.

	\return
		The right audio sample.
	***************************************************************************/
	constexpr SampleType & Right(StereoData& s)
	{
		return s.second;//std::get<1>(s);
	}

	/*! ************************************************************************
	\brief
		Returns the right audio sample from a stereo data pair.

	\param s
		The stereo audio sample.

	\return
		The right audio sample.
	***************************************************************************/
	constexpr SampleType const & Right(StereoData const & s)
	{
		return s.second;//std::get<1>(s);
	}
} // namespace OCAE

#endif // __OCAE_UTIL_HPP
