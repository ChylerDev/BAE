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

#include "Sound.hpp"
#include "Driver.hpp"

// Private Macros                         //////////////////////////////////////

#define PA_ERROR_CHECK(code, badtxt, goodtxt)      \
{                                                  \
  if(code != paNoError)                            \
  {                                                \
    std::cerr << badtxt << '\n';        \
    std::cerr << Pa_GetErrorText(code); \
    exit(-1);                                      \
  }                                                \
  std::cout << goodtxt << '\n';         \
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

      Track_t m_Track;

      std::deque<Track_t> m_NewSamples;

    public:

      template<typename ...Args>
      static inline Recorder_t Create(Args &&... params)
      {
        return std::make_shared<Recorder>(params...);
      }

      // Con-/De- structors   ///////////////////////

      Recorder();
      ~Recorder();

      // Operators            ///////////////////////

      // Accossors/Mutators   ///////////////////////

      // Functions            ///////////////////////

      void SendSamples(Track_t const & samples);

      Track_t GetRecording();

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

  Driver::Driver(Math_t gain) :
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

  void Driver::AddSound(Sound_t const & sound)
  {
    m_Sounds.push_back(sound);
  }

  void Driver::StartRecording()
  {
    m_Recorder = Recorder::Create();
    m_Recording = true;
  }

  Track_t Driver::StopRecording()
  {
    Track_t recording = m_Recorder->GetRecording();

    m_Recording = false;
    m_Recorder = nullptr;

    return recording;
  }

  void Driver::SetGain(Math_t gain)
  {
    m_Gain = gain;
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
        std::cerr << "PortAudio frame count is larger than the allowed buffer size. "
                             << "This is a guaranteed underflow scenario!!!\n";
      }
    #endif

    #ifdef _DEBUG
      static unsigned long old_count = 0;

      if(old_count != frameCount)
      {
        std::cout << "PortAudio buffer size: " << frameCount << '\n';
        old_count = frameCount;
      }
    #endif

      // Convert input data to usable type
    Driver * obj = reinterpret_cast<Driver *>(userData);
    SampleType_t * out = reinterpret_cast<SampleType_t*>(output);

    obj->m_OutputTrack.assign(frameCount, StereoData_t(SampleType_t(0), SampleType_t(0)));

    for(auto & s : obj->m_Sounds)
    {
      s->SendBlock(obj->m_OutputTrack.data(), obj->m_OutputTrack.size());
    }

    static uint64_t i;
    for(i = 0; i < frameCount; ++i)
    {
      out[2*i] = (SampleType_t(Left(obj->m_OutputTrack[i]) * obj->m_Gain));
      out[2*i+1] = (SampleType_t(Right(obj->m_OutputTrack[i]) * obj->m_Gain));
    }

    if(obj->m_Recording)
    {
      obj->m_Recorder->SendSamples(obj->m_OutputTrack);
    }

      // Do over/underflow checks
    if(statusFlags & paOutputUnderflow)
    {
        // UNDERFLOW!!!
      std::cerr << "Audio system is experiencing data underflow, "
                           << "zero-data will be inserted to keep up!\n";
    }
    else if(statusFlags & paOutputOverflow)
    {
        // OVERFLOW!!!
      std::cerr << "Audio system is experiencing data overflow, "
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
    m_Track(), m_NewSamples()
  {
      // Reserve 1 second worth of memory to record
    m_Track.reserve(SAMPLE_RATE);
  }

  Recorder::~Recorder()
  {
  }

  void Recorder::SendSamples(Track_t const & samples)
  {
    m_NewSamples.push_back(samples);
  }

  Track_t Recorder::GetRecording()
  {
    m_Track.reserve(m_NewSamples.size() * SAMPLE_RATE);
    for(auto & s : m_NewSamples)
    {
      m_Track.insert(m_Track.end(), s.begin(), s.end());
    }

    return m_Track;
  }

} // namespace Core
} // namespace AudioEngine
