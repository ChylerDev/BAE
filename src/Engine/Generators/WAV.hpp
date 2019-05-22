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
#include <memory>
#include <vector>

#include "../Engine.hpp"

#include "Base.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{

  /*! **************************************************************************
  \brief
    Plays audio from a WAV file.

    Supported formats: 8-bit, 16-bit, and 24-bit audio. 48kHz only for now,
    resampler will be added soon.
  *****************************************************************************/
  class WAV : public Base
  {
  private:

    // Members              ///////////////////////

    Resampler_t m_Resampler;

  public:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Default constructor. If no data is provided in calling WAV::ReadFile, then
      WAV::SendSample will only output 0 data.
    ***************************************************************************/
    WAV();

    /*! ************************************************************************
    \brief
      Path to a WAV file.

    \param path
      The path.
    ***************************************************************************/
    WAV(std::string const & path);

    /*! ************************************************************************
    \brief
      std::vector with the contents of a WAV file.

    \param wav_data
      The WAV data
    ***************************************************************************/
    WAV(std::vector<char> const & wav_data);

    /*! ************************************************************************
    \brief
      Integer argc parameter passed into main. Uses the functions in Input.*pp
      to access the command-line parameters.

    \param argc
      Parameter passed into main.
    ***************************************************************************/
    WAV(int argc);

    /*! ************************************************************************
    \brief
      Default destructor.
    ***************************************************************************/
    virtual ~WAV() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    /*! ************************************************************************
    \brief
      Reads a file from the disk and parses it for the WAV data.

    \param path
      The path to the file.
    ***************************************************************************/
    void ReadFile(std::string const & path);

    /*! ************************************************************************
    \brief
      Sends a single sample to Core::Driver for output to the OS.

    \return
      The stereo sample data.
    ***************************************************************************/
    virtual StereoData_t SendSample(void);

  private:

    struct WAVHeader
    {                       // (offset) = description
      uint16_t AudioFormat;     // (00) = 1
      uint16_t ChannelCount;    // (02) = 1 or 2
      uint32_t SamplingRate;    // (04) = (ex. 44.1kHz, 48kHz, 96kHz, 192kHz)
      uint32_t BytesPerSecond;  // (08) = SamplingRate * BytesPerSample
      uint16_t BytesPerSample;  // (12) = BitsPerSample/8 * ChannelCount
      uint16_t BitsPerSample;   // (14) = 8 or 16
    };

    // Functions                  ///////////////////////

    void ParseWAV(char const * array, int size);

  }; // class WAV

} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __STUB_HPP
