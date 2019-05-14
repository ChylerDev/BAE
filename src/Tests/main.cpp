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

  AudioEngine::Core::Driver driver;

  #if 1

    AudioEngine::Generator::Sine sine_data(55);
    std::shared_ptr<AudioEngine::Core::Node> gen;

    if(argc > 1)
    {
      Log::Trace::out[stc] << "Reading from WAV file\n";

      gen = std::make_shared<AudioEngine::Core::Node>(
        AudioEngine::Generator::WAV(AudioEngine::Tools::GetOptions().at(1))
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

    // std::shared_ptr<AudioEngine::Core::Sound> s = std::make_shared<AudioEngine::Core::Sound>();
    // s->AddNode(gen, 0, true);
    // driver.AddSound(s);

  #else

    AudioEngine::Generator::Square root(110);
    AudioEngine::Generator::Square first(220);
    AudioEngine::Generator::Square second(440);
    AudioEngine::Generator::Square third(880);
    AudioEngine::Generator::Square fourth(1760);
    AudioEngine::Generator::Square fifth(3520);

    AudioCallback_t ref_r([&root](){ return root.SendSample(); });
    AudioCallback_t ref_1([&first](){ return first.SendSample(); });
    AudioCallback_t ref_2([&second](){ return second.SendSample(); });
    AudioCallback_t ref_3([&third](){ return third.SendSample(); });
    AudioCallback_t ref_4([&fourth](){ return fourth.SendSample(); });
    AudioCallback_t ref_5([&fifth](){ return fifth.SendSample(); });

    driver.AddAudioCallback(ref_r);
    driver.AddAudioCallback(ref_1);
    driver.AddAudioCallback(ref_2);
    driver.AddAudioCallback(ref_3);
    driver.AddAudioCallback(ref_4);
    driver.AddAudioCallback(ref_5);

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
