/*! ****************************************************************************
\file             WAVWriter.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "WAVHeader.hpp"
#include "WAVWriter.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Tools
{
	RIFF::vector_t WriteWAV(Track_t const & audio)
	{
		RIFF::Writer writer(CONSTRUCT_BYTE_STR("WAVE"));

		WAVHeader header;

		RIFF::vector_t format;
		format.assign(
			reinterpret_cast<RIFF::byte_t *>(&header),
			reinterpret_cast<RIFF::byte_t *>(&header) + sizeof(WAVHeader)
		);

		RIFF::vector_t data;
		data.reserve(audio.size() * 4);

		for(auto & a : audio)
		{
			data.push_back(
				static_cast<RIFF::byte_t>(
					(static_cast<uint16_t>(int16_t(Left(a)*0x8000)) & 0x00'FF)
				)
			);
			data.push_back(
				static_cast<RIFF::byte_t>(
					(static_cast<uint16_t>(int16_t(Left(a)*0x8000)) & 0xFF'00) >> 8
				)
			);
			data.push_back(
				static_cast<RIFF::byte_t>(
					(static_cast<uint16_t>(int16_t(Right(a)*0x8000)) & 0xFF)
				)
			);
			data.push_back(
				static_cast<RIFF::byte_t>(
					(static_cast<uint16_t>(int16_t(Right(a)*0x8000)) & 0xFF'00) >> 8
				)
			);
		}

		writer.AddChunk(CONSTRUCT_BYTE_STR("fmt "), format);
		writer.AddChunk(CONSTRUCT_BYTE_STR("data"), data);

		return writer.RIFFData();
	}
} // namespace Tools
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
