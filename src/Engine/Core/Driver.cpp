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
#include <mutex>
#include <queue>
#include <thread>

#include <portaudio/portaudio.h>
#include <Trace/Trace.hpp>

#include "../Engine.hpp"
#include "Driver.hpp"
#include "../Modifiers/Vocoder.hpp"
#include "../Modifiers/Envelope.hpp"

// Private Macros                         //////////////////////////////////////

#define PA_ERROR_CHECK(code, badtxt, goodtxt)      \
{                                                  \
  if(code != paNoError)                            \
  {                                                \
    Log::Trace::out[err] << badtxt << '\n';        \
    Log::Trace::out[err] << Pa_GetErrorText(code); \
    exit(-1);                                      \
  }                                                \
  Log::Trace::out[stc] << goodtxt << '\n';         \
}                                                  \

// Private Enums                          //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

namespace AudioEngine
{
namespace Core
{

  class Recorder
  {
    private:

      // Members              ///////////////////////

      std::mutex m_TrackLock;
      Track_t m_Track;

      std::mutex m_SampleLock;
      std::queue<Track_t> m_NewSamples;

      bool m_Recording;
      std::thread m_Runner;

    public:

      // Con-/De- structors   ///////////////////////

      Recorder();
      ~Recorder();

      // Operators            ///////////////////////

      // Accossors/Mutators   ///////////////////////

      // Functions            ///////////////////////

      void SendSamples(Track_t const & samples);

      Track_t GetRecording();

    private:

      // Functions                  ///////////////////////

      void Runner();

  };

  //static AudioEngine::Modifier::Vocoder v(4);

} // namespace Core
} // namespace AudioEngine

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Core
{

PUSH_WARNINGS()
GCC_DISABLE_WARNING("-Wold-style-cast")

  Driver::Driver(float gain) :
    m_Params(), m_Stream(nullptr), m_OutputTrack(), m_AudioCallbacks(),
    m_Sounds(), m_Gain(gain), m_Running(true), m_Recorder(), m_Recording(false)
  {
    m_OutputTrack.reserve(MAX_BUFFER);

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

POP_WARNINGS()

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

  void Driver::AddSound(std::shared_ptr<Sound> const & sound)
  {
    m_Sounds.push_back(sound);
  }

  void Driver::StartRecording()
  {
    m_Recorder = std::make_shared<Recorder>();
    m_Recording = true;
  }

  Track_t Driver::StopRecording()
  {
    Track_t recording = m_Recorder->GetRecording();

    m_Recording = false;
    m_Recorder = nullptr;

    return recording;
  }

  void Driver::Shutdown()
  {
    m_Running = false;
  }

} // namespace Core
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////

namespace AudioEngine
{
namespace Core
{

PUSH_WARNINGS()
GCC_DISABLE_WARNING("-Wold-style-cast")

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
    
      // Convert input data to usable type
    Driver * obj = reinterpret_cast<Driver *>(userData);
    float * out = reinterpret_cast<float *>(output);

    obj->m_OutputTrack.clear();

      // Loop through the buffer, copying the data to the output
    uint64_t i = 0;
    for(; i < frameCount; ++i)
    {
      StereoData_t sum(0.f,0.f);
      for(auto cb : obj->m_AudioCallbacks)
      {
        StereoData_t y = cb();
        std::get<0>(sum) += std::get<0>(y);
        std::get<1>(sum) += std::get<1>(y);
      }
      for(auto & s : obj->m_Sounds)
      {
        StereoData_t y = s->GetSample();
        std::get<0>(sum) += std::get<0>(y);
        std::get<1>(sum) += std::get<1>(y);
      }

      out[2*i] = std::get<0>(sum) * obj->m_Gain;
      out[2*i+1] = std::get<1>(sum) * obj->m_Gain;

      obj->m_OutputTrack.push_back(sum);
    }

    if(obj->m_Recording)
    {
      obj->m_Recorder->SendSamples(obj->m_OutputTrack);
    }

      // Do over/underflow checks
    if(i < frameCount || statusFlags & paOutputUnderflow)
    {
        // UNDERFLOW!!!
      Log::Trace::out[err] << "Audio system is experiencing data underflow, "
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

    if(obj->m_Running)
    {
      return paContinue;
    }
    return paComplete;
  }

POP_WARNINGS()

  //////// Recorder Functions ////////

  Recorder::Recorder() :
    m_TrackLock(), m_Track(),
    m_SampleLock(), m_NewSamples(),
    m_Recording(true), m_Runner([this](){ this->Runner(); })
  {
      // Reserve 1 second worth of memory to record
    m_Track.reserve(SAMPLE_RATE);
  }

  Recorder::~Recorder()
  {
  }

  void Recorder::SendSamples(Track_t const & samples)
  {
    m_SampleLock.lock();
    m_NewSamples.push(samples);
    m_SampleLock.unlock();
  }

  Track_t Recorder::GetRecording()
  {
    m_Recording = false;
    m_Runner.join();

    m_TrackLock.lock();
    m_TrackLock.unlock();

    return m_Track;
  }

  void Recorder::Runner()
  {
    do
    {
      size_t size;
      Track_t samples;

      m_SampleLock.lock();
      size = m_NewSamples.size();
      m_SampleLock.unlock();

      while(size)
      {
        m_SampleLock.lock();
        samples = m_NewSamples.front();
        m_NewSamples.pop();
        m_SampleLock.unlock();

        m_TrackLock.lock();
        m_Track.insert(m_Track.end(), samples.begin(), samples.end());
        m_TrackLock.unlock();

        --size;
      }

      std::this_thread::yield();
    } while(m_Recording);
  }

} // namespace Core
} // namespace AudioEngine
