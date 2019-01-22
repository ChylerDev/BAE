/*! ****************************************************************************
\file             Driver.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cstring>
#include <iostream>

#include <portaudio/portaudio.h>

#include "../Engine.hpp"

#include "../Trace/src/Trace.hpp"
#include "Driver.hpp"

// Private Macros                         //////////////////////////////////////

#define PA_ERROR_CHECK(code, badtxt, goodtxt) \
{                                             \
  if(code != paNoError)                       \
  {                                           \
    Log::Trace::out[err] << badtxt << '\n';        \
    Log::Trace::out[err] << Pa_GetErrorText(code); \
    exit(-1);                                 \
  }                                           \
  Log::Trace::out[stc] << goodtxt << '\n';         \
}                                             \

// Private Enums                          //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace Core
{

  Driver::Driver(float gain) : m_BufferSize(0), m_Gain(gain), m_Running(true)
  {
    auto code = Pa_Initialize();
    PA_ERROR_CHECK(code, "PortAudio failed to initialize", "PortAudio initialized");

    m_Params.device = Pa_GetDefaultOutputDevice();
    m_Params.channelCount = 2;
    m_Params.sampleFormat = paFloat32;
    m_Params.suggestedLatency = Pa_GetDeviceInfo(m_Params.device)->defaultHighOutputLatency;
    m_Params.hostApiSpecificStreamInfo = nullptr;

    code = Pa_OpenStream(&m_Stream, nullptr, &m_Params, SAMPLE_RATE, 0, 0,
                         &Driver::s_WriteCallback, this);
    PA_ERROR_CHECK(code, "Failed to open the audio stream", "Opened the audio stream");

    code = Pa_StartStream(m_Stream);
    PA_ERROR_CHECK(code, "Failed to start the audio stream", "Started the audio stream");
  }

  Driver::~Driver()
  {
    auto code = Pa_CloseStream(m_Stream);
    PA_ERROR_CHECK(code, "Failed to close the audio stream", "Closed the audio stream");

    code = Pa_Terminate();
    PA_ERROR_CHECK(code, "PortAudio failed to terminate", "PortAudio terminated");
  }

  void Driver::AddAudioCallback(AudioCallback_t const & cb)
  {
    m_AudioCallbacks.push_back(cb);
  }

  void Driver::Shutdown()
  {
    m_Running = false;
  }

} // namespace Core

// Private Functions                      //////////////////////////////////////

namespace Core
{

  int Driver::s_WriteCallback(void const * input, void * output,
                              unsigned long frameCount,
                              PaStreamCallbackTimeInfo const * timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void * userData)
  {
    UNREFERENCED_PARAMETER(timeInfo);
    UNREFERENCED_PARAMETER(input);

  #ifdef _DEBUG
    if(frameCount > MAX_BUFFER)
    {
      Log::Trace::out[err] << "PortAudio frame count is larger than the allowed buffer size. "
                           << "This is a guaranteed underflow scenario!!!\n";
    }
  #endif

    static unsigned long old_count = 0;

    if(old_count != frameCount)
    {
      Log::Trace::out[frq] << "PortAudio buffer size: " << frameCount << '\n';
      old_count = frameCount;
    }
    
      // Convert input data to usable type    s
    Driver * obj = reinterpret_cast<Driver *>(userData);
    float * out = reinterpret_cast<float *>(output);

      // Loop through the buffer, copying the data to the output
    uint64_t i = 0;
    for(; i < frameCount && i < obj->m_BufferSize; ++i)
    {
      StereoData_t sum(0.f,0.f);
      for(auto cb : obj->m_AudioCallbacks)
      {
        StereoData_t y = cb();
        std::get<0>(sum) += std::get<0>(y);
        std::get<1>(sum) += std::get<1>(y);
      }
      out[2*i] = std::get<0>(sum) * obj->m_Gain;
      out[2*i+1] = std::get<1>(sum) * obj->m_Gain;
    }

      // Do over/underflow checks
    if(i < frameCount || statusFlags & paOutputUnderflow)
    {
        // UNDERFLOW!!!
      Log::Trace::out[err] << "Audio system is experiencing data undeflow, "
                           << "zero-data will be inserted to keep up!\n";

      while(i < frameCount)
      {
        out[i++] = 0.f;
      }
    }
    else if(statusFlags & paOutputOverflow)
    {
        // OVERFLOW!!!
      Log::Trace::out[err] << "Audio system is experiencing data overflow, "
                           << "some data will be discarded to keep up!\n";
    }

    obj->m_BufferSize -= i;

    if(obj->m_Running)
    {
      return paContinue;
    }
    return paComplete;
  }

} // namespace Core
