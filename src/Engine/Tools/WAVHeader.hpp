/*! ****************************************************************************
\file             WAVHeader.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __WAVHEADER_HPP
#define __WAVHEADER_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{
	struct WAVHeader
	{                         // (offset) = description
		uint16_t AudioFormat;     // (00) = 1
		uint16_t ChannelCount;    // (02) = 1 or 2
		uint32_t SamplingRate;    // (04) = (ex. 44.1kHz, 48kHz, 96kHz, 192kHz)
		uint32_t BytesPerSecond;  // (08) = SamplingRate * BytesPerSample
		uint16_t BytesPerSample;  // (12) = BitsPerSample/8 * ChannelCount
		uint16_t BitsPerSample;   // (14) = 8 or 16

		WAVHeader(
			uint16_t af = 1,			// Audio format
			uint16_t cc = 2,			// Channel count
			uint32_t R = SAMPLE_RATE,	// Sampling rate
			uint16_t bps = 16			// Bits per sample
		) : AudioFormat(af), ChannelCount(cc), SamplingRate(R),
			BytesPerSecond((bps >> 3) * cc * R), BytesPerSample((bps >> 3) * cc),
			BitsPerSample(bps)
		{};
	};
} // namespace Tools
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __WAVHEADER_HPP
