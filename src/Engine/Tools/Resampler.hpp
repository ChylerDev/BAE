/*! ****************************************************************************
\file             Resampler.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __RESAMPLER_HPP
#define __RESAMPLER_HPP

// Include Files                ////////////////////////////////////////////////

#include <memory>
#include <vector>

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class Resampler
  {
  private:

    using Index_t = Math::FixedPoint<23,8>;

    // Members              ///////////////////////

    std::vector<StereoData_t> const & m_Data;

    Index_t m_Index;
    Math_t const m_IndexIncrement;

    uint64_t m_LoopStart, m_LoopEnd;

  public:

    template<typename ...Args>
    static inline pResampler_t Create(Args &&... params)
    {
      return std::make_shared<Resampler>(params...);
    }

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Constructor for the resampler.

    \param AudioData
      A const reference to the audio data.

    \param SourceSampleRate
      The sample rate of the source data.

    \param LoopStart
      The sample to start looping from. Defaults to 0.

    \param LoopEnd
      The sample at the loop point to loop back to LoopStart. Defaults to 0,
      which is interpretted as no looping.
    ***************************************************************************/
    Resampler(std::vector<StereoData_t> const & AudioData,
              int32_t SourceSampleRate,
              uint64_t LoopStart = 0, uint64_t LoopEnd = 0
    );

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    /*! ************************************************************************
    \brief
      Sends a single sample to Core::Driver for output to the OS.

    \return
      The stereo sample data.
    ***************************************************************************/
    StereoData_t SendSample();

  private:

    // Functions                  ///////////////////////

  }; // class Resampler

} // namespace Tools
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __RESAMPLER_HPP
