/*! ****************************************************************************
\file             WAVHeader.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "WAVHeader.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{
	WAVHeader::WAVHeader(uint16_t af, uint16_t cc, uint32_t R, uint16_t bps):
		AudioFormat(af), ChannelCount(cc), SamplingRate(R),
		BytesPerSecond((bps >> 3) * cc * R), BytesPerSample(uint16_t((bps >> 3) * cc)),
		BitsPerSample(bps)
	{	}
} // namespace Tools
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
