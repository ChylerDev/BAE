/*! ****************************************************************************
\file             WAVHeader.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __OCAE_WAVHEADER_HPP
#define __OCAE_WAVHEADER_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Tools
{
	/*! ************************************************************************
	\brief
		A POD structure representing the structure of the header of a WAVE file.
	***************************************************************************/
	struct WAVHeader
	{                         // (offset) = description
		uint16_t AudioFormat;     ///< Offset 00 = 1
		uint16_t ChannelCount;    ///< Offset 02 = 1 or 2
		uint32_t SamplingRate;    ///< Offset 04 = (ex. 44.1kHz, 48kHz, 96kHz, 192kHz)
		uint32_t BytesPerSecond;  ///< Offset 08 = SamplingRate * BytesPerSample
		uint16_t BytesPerSample;  ///< Offset 12 = BitsPerSample/8 * ChannelCount
		uint16_t BitsPerSample;   ///< Offset 14 = 8 or 16

		/*! ********************************************************************
		\brief
			Consturctor for a WAVE header, with default values for standard
			16-bit audio data.

		\param af
			The audio format, should generally be left at 1.

		\param cc
			The channel count. OCAE uses two-channel audio.

		\param R
			The sampling rate. OCAE uses OCAE_SAMPLE_RATE (probably defined as 48kHz).

		\param bps
			Bits per audio sample. We are using 16-bit audio as it is all of the
			quality you should need.
		***********************************************************************/
		WAVHeader(
			uint16_t af = 1,			// Audio format
			uint16_t cc = 2,			// Channel count
			uint32_t R = OCAE_SAMPLE_RATE,	// Sampling rate
			uint16_t bps = 16			// Bits per sample
		);
		~WAVHeader() = default;	///< Default destructor.
	};
} // namespace Tools
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __OCAE_WAVHEADER_HPP
