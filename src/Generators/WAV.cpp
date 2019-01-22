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

#include "../Trace/src/Trace.hpp"

#include "WAV.hpp"
#include "../Tools/Input.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace Generator
{

  WAV::WAV() : m_Data(), m_CurrentIndex(0)
  {
  }

  WAV::WAV(std::string const & path) : m_Data(), m_CurrentIndex(0)
  {
    ReadFile(path);
  }

  WAV::WAV(std::vector<char> const & data) : m_Data(), m_CurrentIndex(0)
  {
    ParseWAV(data.data(), int(data.size()));
  }

  WAV::WAV(int argc) : m_Data(), m_CurrentIndex(0)
  {
    std::string path = GetOptions().at(argc);

    ReadFile(path);
  }

  StereoData_t WAV::SendSample()
  {
    if(m_CurrentIndex < m_Data.size())
    {
      return m_Data[m_CurrentIndex++];
    }
    return StereoData_t(0.f,0.f);
  }

} // namespace Generator

// Private Functions                      //////////////////////////////////////

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

      temp.assign((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());

      ParseWAV(temp.data(), int(temp.size()));
    }
  }

  void WAV::ParseWAV(char const * array, int size)
  {
    WAVHeader const * header = reinterpret_cast<WAVHeader const *>(array);

      // Do safety checks
    auto res1 = std::strncmp(header->RiffLabel, "RIFF", 4);
    auto res2 = std::strncmp(header->FileTag, "WAVE", 4);
    auto res3 = std::strncmp(header->FmtLabel, "fmt ", 4);
    if(res1 || res2 || res3)
    {
      Log::Trace::out[err] << "Wave file read incorrectly. Header is non-comforming. Substituting zero-data\n";
      return;
    }

      // Search for the data chunk
    char const * walk = array + 36;
    while(walk < array + size && std::strncmp(walk, "data", 4))
    {
      walk += 4;
      walk += *reinterpret_cast<uint32_t const *>(walk + 4) + 4;
    }
      // Error check
    if(walk >= array+size)
    {
      Log::Trace::out[err] << "Wave file missing data chunk. Wave file is non-conforming. Substituting zero-data\n";
      return;
    }

    uint32_t BytesPerSample = header->BitsPerSample/8 * header->ChannelCount;

    walk += 4;
    uint32_t DataSize = header->DataSize;
    walk += 4;
    m_Data.reserve(DataSize / BytesPerSample);

      // Parse data
    char const * data = walk;
    while(data < walk + DataSize)
    {
      StereoData_t sample;

      if(header->BitsPerSample == 8)
      {
        std::get<0>(sample) = (*data - 128)/128.f;
        std::get<1>(sample) = (*(header->ChannelCount==1 ? data : data+1) - 128)/128.f;
      }
      else
      {
        int16_t const * rdata = reinterpret_cast<int16_t const *>(data);
        std::get<0>(sample) = (*rdata)/float(0x7FFF);
        std::get<1>(sample) = (*(header->ChannelCount==1 ? rdata : rdata+1))/float(0x7FFF);
      }

      m_Data.push_back(sample);
      data += BytesPerSample;
    }
  }

} // namespace Generator
