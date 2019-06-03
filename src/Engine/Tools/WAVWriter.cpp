/*! ****************************************************************************
\file             WAVWriter.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "WAVWriter.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

struct WAVHeader
{                       // (offset) = description
  uint16_t AudioFormat;     // (00) = 1
  uint16_t ChannelCount;    // (02) = 1 or 2
  uint32_t SamplingRate;    // (04) = (ex. 44.1kHz, 48kHz, 96kHz, 192kHz)
  uint32_t BytesPerSecond;  // (08) = SamplingRate * BytesPerSample
  uint16_t BytesPerSample;  // (12) = BitsPerSample/8 * ChannelCount
  uint16_t BitsPerSample;   // (14) = 8 or 16
};

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{

  RIFF::vector_t WriteWAV(Track_t const & audio)
  {
    RIFF::Writer writer(CONSTRUCT_BYTE_STR("WAVE"));

    WAVHeader header;
    header.AudioFormat = 1;
    header.ChannelCount = 2;
    header.SamplingRate = SAMPLE_RATE;
    header.BitsPerSample = 16;
    header.BytesPerSample = uint16_t((header.BitsPerSample >> 3) * header.ChannelCount);
    header.BytesPerSecond = SAMPLE_RATE * header.BytesPerSample;

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
          static_cast<uint16_t>(std::get<0>(a).Data()) & 0x00'FF
        )
      );
      data.push_back(
        static_cast<RIFF::byte_t>(
          (static_cast<uint16_t>(std::get<0>(a).Data()) & 0xFF'00) >> 8
        )
      );
      data.push_back(
        static_cast<RIFF::byte_t>(
          static_cast<uint16_t>(std::get<1>(a).Data()) & 0xFF
        )
      );
      data.push_back(
        static_cast<RIFF::byte_t>(
          (static_cast<uint16_t>(std::get<1>(a).Data()) & 0xFF'00) >> 8
        )
      );
    }

    writer.AddChunk(CONSTRUCT_BYTE_STR("fmt "), format);
    writer.AddChunk(CONSTRUCT_BYTE_STR("data"), data);

    return writer.RIFFData();
  }

} // namespace Tools
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
