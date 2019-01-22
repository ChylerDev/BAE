/*! ****************************************************************************
\file             WAV.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __WAV_HPP
#define __WAV_HPP

// Include Files                ////////////////////////////////////////////////

#include <string>
#include <vector>

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace Generator
{

/*! ****************************************************************************
\brief
  Plays audio from a WAV file.

  Supported formats: 8-bit, 16-bit, and 24-bit audio. 48kHz only for now,
  resampler will be added soon.
*******************************************************************************/
class WAV
{
private:

  // Members              ///////////////////////

  std::vector<StereoData_t> m_Data;
  uint64_t m_CurrentIndex;

public:

  // Con-/De- structors   ///////////////////////

  WAV() : m_Data(), m_CurrentIndex(0) {};
  WAV(std::string const & path);
  WAV(std::vector<char> const & wav_data);
  WAV(int argc);

  // Operators            ///////////////////////

  // Accossors/Mutators   ///////////////////////

  // Functions            ///////////////////////

  void ReadFile(std::string const & path);

  StereoData_t SendSample(void);

private:

  struct WAVHeader
  {
    char RiffLabel[4];        // (00) = {'R','I','F','F'}
    uint32_t RiffSize;        // (04) = 36 + data_size
    char FileTag[4];          // (08) = {'W','A','V','E'}
    char FmtLabel[4];         // (12) = {'f','m','t',' '}
    uint32_t FmtSize;         // (16) = 16
    uint16_t AudioFormat;     // (20) = 1
    uint16_t ChannelCount;    // (22) = 1 or 2
    uint32_t SamplingRate;    // (24) = (anything)
    uint32_t BytesPerSecond;  // (28) = (see above)
    uint16_t BytesPerSample;  // (32) = (see above)
    uint16_t BitsPerSample;   // (34) = 8 or 16
    char DataLabel[4];        // (36) = {'d','a','t','a'}
    uint32_t DataSize;        // (40) = # bytes of data
  };

  // Functions                  ///////////////////////


  void ParseWAV(char const * array, int size);

}; // class WAV

} // namespace Generator

// Public Functions             ////////////////////////////////////////////////

#endif // __STUB_HPP
