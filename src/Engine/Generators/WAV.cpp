/*! ****************************************************************************
\file             WAV.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cstring>

#include <fstream>
#include <sstream>

#include <Trace/Trace.hpp>
#include <RIFF-Util/Riff.hpp>

#include "../Tools/Input.hpp"
#include "../Tools/Resampler.hpp"

#include "WAV.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{

  WAV::WAV() : Base(false),
    m_Data(), m_CurrentIndex(0), m_Resampler()
  {
  }

  WAV::WAV(std::string const & path) : Base(false),
    m_Data(), m_CurrentIndex(0), m_Resampler()
  {
    ReadFile(path);
  }

  WAV::WAV(std::vector<char> const & data) : Base(false),
    m_Data(), m_CurrentIndex(0), m_Resampler()
  {
    ParseWAV(data.data(), int(data.size()));
  }

  WAV::WAV(int argc) : Base(false),
    m_Data(), m_CurrentIndex(0), m_Resampler()
  {
    ReadFile(Tools::GetOptions().at(argc));
  }

  StereoData_t WAV::SendSample()
  {
    if(m_CurrentIndex++ < m_Data.size() && m_Resampler)
    {
      return m_Resampler->SendSample();
    }
    return StereoData_t(0,0);
  }

} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{

  void WAV::ReadFile(std::string const & path)
  {
    std::ifstream l_file(path, std::ios::binary);

    if(!l_file)
    {
      Log::Trace::out[err] << "WAV file " << path << " couldn't be opened.\n";
    }
    else
    {
      Log::Trace::out[stc] << "WAV file " << path << " opened for reading\n";

      std::vector<char> temp;

      l_file.seekg(0, std::ios::end);
      temp.reserve(l_file.tellg());
      l_file.seekg(0, std::ios::beg);

      Log::Trace::out[stc] << "Loading file...\n";
      Log::Trace::out.flush();

      temp.assign((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());

      Log::Trace::out[stc] << "File successfully loaded\n";

      ParseWAV(temp.data(), int(temp.size()));
    }
  }

  void WAV::ParseWAV(char const * array, int size)
  {
      // Copy RIFF data to byte vector
    RIFF::vector_t vec;
    vec.assign(CHAR_STR_TO_BYTE_STR(array), CHAR_STR_TO_BYTE_STR(array) + size);
      // Parse the riff data
    RIFF::Reader riff(vec, CONSTRUCT_BYTE_STR("WAVE"));

      // Get the format chunk, check that it's size is correct
    RIFF::vector_t fmt = riff.GetChunk(CONSTRUCT_BYTE_STR("fmt "));
    if(fmt.size() != sizeof(WAVHeader))
    {
      Log::Trace::out[err] << "Error reading WAVE data, malformed header chunk\n";
      return;
    }
      // Cast the data as a header object for easier use
    WAVHeader const * header = reinterpret_cast<WAVHeader const *>(fmt.data());

      // Get the data chunk
    RIFF::vector_t data_vec = riff.GetChunk(CONSTRUCT_BYTE_STR("data"));
    m_Data.reserve(data_vec.size() / header->BytesPerSample);

      // Parse data
    char const * data = reinterpret_cast<char *>(&data_vec[0]);
    while(data < reinterpret_cast<char *>(&data_vec[0] + data_vec.size()))
    {
      StereoData_t sample;

      if(header->BitsPerSample == 8)
      {
        std::get<0>(sample).Data() = (*data) << 8;
        std::get<1>(sample).Data() = (*(header->ChannelCount==1 ? data : data+1)) << 8;
      }
      else
      {
        int16_t const * rdata = reinterpret_cast<int16_t const *>(data);
        std::get<0>(sample).Data() = *rdata;
        std::get<1>(sample).Data() = (*(header->ChannelCount==1 ? rdata : rdata+1));
      }

      m_Data.push_back(sample);
      data += header->BytesPerSample;
    }

    m_Resampler = Resampler_t::Create(
      m_Data, header->SamplingRate, 0, m_Data.size()-1
    );
  }

} // namespace Generator
} // namespace AudioEngine
