/*! ****************************************************************************
\file             Driver.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __DRIVER_HPP
#define __DRIVER_HPP

// Include Files                ////////////////////////////////////////////////

#include <functional>
#include <memory>
#include <vector>

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

#ifndef MAX_BUFFER
  #define MAX_BUFFER (SAMPLE_RATE/10)
#endif

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Core
{

  /*! **************************************************************************
  \brief
    Initiates Port Audio and handles all related calls.
  *****************************************************************************/
  class Driver
  {
  private:

    // Members              ///////////////////////

    PaStreamParameters m_Params;
    PaStream * m_Stream;

    Track_t m_OutputTrack;

    std::vector<AudioCallback_t> m_AudioCallbacks;
    std::vector<Sound_t> m_Sounds;

    Math_t m_Gain;
    bool m_Running;

    Recorder_t m_Recorder;
    bool m_Recording;

  public:

    template<typename ...Args>
    static inline Driver_t Create(Args &&... params)
    {
      return std::make_shared<Driver>(params...);
    }

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Constructs an audio driver object.

    \param gain
      The linear gain to be used when summing all audio values.
    ***************************************************************************/
    Driver(Math_t gain = DEFAULT_GAIN);

    Driver(Driver const &) = delete;
    Driver(Driver &&) noexcept = default;

    /*! ************************************************************************
    \brief
      Destructor.
    ***************************************************************************/
    ~Driver();

    // Operators            ///////////////////////

    Driver & operator=(Driver const &) = delete;
    Driver & operator=(Driver &&) noexcept = default;

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    /*! ************************************************************************
    \brief
      Adds an audio callback to the list of callbacks to check.

    \param cb
      The callback to be added to the list.
    ***************************************************************************/
    void AddAudioCallback(AudioCallback_t const & cb);

    void AddSound(Sound_t const & sound);

    void StartRecording();

    Track_t StopRecording();

    /*! ************************************************************************
    \brief
      Sets the gain to be used when summing all the audio values.

    \param gain
      The linear gain value to be set.
    ***************************************************************************/
    void SetGain(Math_t gain = DEFAULT_GAIN);

    /*! ************************************************************************
    \brief
      Sets the threaded runner to stop grabbing data.
    ***************************************************************************/
    void Shutdown();

  private:

    // Functions                  ///////////////////////

    /*! ************************************************************************
    \brief
      Callback function for writing data to the speakers or reding data from the
      microphones.

    \param input
      Array of input audio data.

    \param output
      Array of output audio data.

    \param frameCount
      The number of sample frames to be processed by the stream callback.

    \param timeInfo
      Timesamps indicating the ADC capture time of the first sample in the input
      buffer, the DAC output time of the first sample in the output buffer, and
      the time the callback was invoked.

    \param statusFlags
      Flags indicating whether input/output buffers have been inserted or will
      be dropped to overcome underflow or overflow conditions.

    \param userData
      The the user-supplied data given when Pa_OpenStream was called.

    \return
      A PaStreamCallbackResult enum value.
    ***************************************************************************/
    static int s_WriteCallback(void const * input, void * output,
                               unsigned long frameCount,
                               PaStreamCallbackTimeInfo const * timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void * userData);
  }; // class Driver

} // namespace Core
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __DRIVER_HPP
