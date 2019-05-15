/*! ****************************************************************************
\file             main.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#include "../Engine/Engine.hpp"

#include "../Engine/Core/Sound.hpp"
#include "../Engine/Generators/Sine.hpp"
#include "../Engine/Generators/Square.hpp"
#include "../Engine/Generators/WAV.hpp"
#include "../Engine/Modifiers/Vocoder.hpp"
#include "../Engine/Tools/Input.hpp"

#include <Trace/Trace.hpp>
#include <RIFF-Util/RIFF.hpp>

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

int main(int argc, char * argv[])
{
  AudioEngine::Tools::CreateOptions(argc, argv);

  AudioEngine::Core::Driver driver(0.125f);

  #if 0

    AudioEngine::Generator::Sine sine_data(55);
    std::shared_ptr<AudioEngine::Core::Node> gen;

    if(argc > 1)
    {
      Log::Trace::out[stc] << "Reading from WAV file\n";

      gen = std::make_shared<AudioEngine::Core::Node>(
        std::make_shared<AudioEngine::Generator::WAV>(AudioEngine::Tools::GetOptions().at(1))
      );
    }
    else
    {
      Log::Trace::out[stc] << "Generating sine tone\n";
      gen = std::make_shared<AudioEngine::Core::Node>(
        std::make_shared<AudioEngine::Generator::Sine>(55.f)
      );
    }

    AudioEngine::Modifier::Vocoder v(gen, 4);
    driver.AddSound(v);

  #else

    using Tone_t = AudioEngine::Generator::Square;
    using AudioEngine::Core::Sound;
    using AudioEngine::Core::Node;

    std::shared_ptr<Sound> sound = std::make_shared<Sound>();

    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(220.f)), 0, true);
    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(440.f)), 0, true);
    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(660.f)), 0, true);
    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(880.f)), 0, true);
    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(1100.f)), 0, true);
    sound->AddNode(std::make_shared<Node>(std::make_shared<Tone_t>(1320.f)), 0, true);

    driver.AddSound(sound);

  #endif

  //driver.StartRecording();

  std::cin.get();

  // AudioEngine::Track_t recording = driver.StopRecording();
  // driver.Shutdown();

  // RIFF::Writer wav_writer(CONSTRUCT_BYTE_STR("WAVE"));

  // WAVHeader header;
  // header.AudioFormat = 1;
  // header.ChannelCount = 2;
  // header.SamplingRate = SAMPLE_RATE;
  // header.BitsPerSample = 16;
  // header.BytesPerSample = header.BitsPerSample / 8 * header.ChannelCount;
  // header.BytesPerSecond = SAMPLE_RATE * header.BytesPerSample;

  // RIFF::vector_t format;
  // format.assign(
  //   reinterpret_cast<RIFF::byte_t *>(&header),
  //   reinterpret_cast<RIFF::byte_t *>(&header) + sizeof(WAVHeader)
  // );

  // std::vector<std::tuple<int16_t,int16_t>> audio;
  // for(auto & f : recording)
  // {
  //   audio.push_back(
  //     std::make_tuple(
  //       int16_t(std::get<0>(f) * 0x7FFF),
  //       int16_t(std::get<1>(f) * 0x7FFF)
  //     )
  //   );
  // }

  // RIFF::vector_t data;
  // for(auto & a : audio)
  // {
  //   data.push_back(static_cast<RIFF::byte_t>(static_cast<uint16_t>(std::get<0>(a)) & 0xFF));
  //   data.push_back(static_cast<RIFF::byte_t>((static_cast<uint16_t>(std::get<0>(a)) & 0xFF00) >> 8));
  //   data.push_back(static_cast<RIFF::byte_t>(static_cast<uint16_t>(std::get<1>(a)) & 0xFF));
  //   data.push_back(static_cast<RIFF::byte_t>((static_cast<uint16_t>(std::get<1>(a)) & 0xFF00) >> 8));
  // }

  // wav_writer.AddChunk(CONSTRUCT_BYTE_STR("fmt "), format);
  // wav_writer.AddChunk(CONSTRUCT_BYTE_STR("data"), data);

  // RIFF::vector_t RIFFData = wav_writer.RIFFData();

  // std::basic_ofstream<RIFF::byte_t> file("wave_out.wav", std::ios_base::binary);

  // file << RIFFData;

  return 0;
}

// Private Functions                      //////////////////////////////////////
