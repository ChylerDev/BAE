/*! ****************************************************************************
\file             Types.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __TYPES_HPP
#define __TYPES_HPP

// Include Files                ////////////////////////////////////////////////

#include <cstdint>

#include <functional>
#include <memory>
#include <tuple>

#define FIXED_POINT_RAW_ACCESS
#include <FixedPoint/Math.hpp>

// Public Macros                ////////////////////////////////////////////////

#define PTR(x) std::shared_ptr<x>

// Forward References           ////////////////////////////////////////////////

namespace AudioEngine
{
  namespace Core
  {

    class Recorder;
    class Driver;
    class Node;
    class Sound;

  } // namespace Core
  namespace Generator
  {

    class Base;
    class Sine;
    class Square;
    class WAV;

  } // namespace Generator
  namespace Modifier
  {

    class Base;
    class BandPass;
    class EnvelopeFollower;
    class Gain;
    class GenericFilter;
    class LowPass;

  } // namespace Modifier
  namespace Sounds
  {

    class Base;
    class Vocoder;

  }
  namespace Tools
  {

    class Resampler;

  } // namespace Tools
} // namespace AudioEngine

// Public Types                 ////////////////////////////////////////////////

namespace AudioEngine
{

  using Math_t = Math::FixedPoint<15,16>;
  using SampleType_t = Math::Audio16Bit_t;
    // StereoData_t[0] = left; StereoData_t[1] = right;
  using StereoData_t = std::tuple<SampleType_t,SampleType_t>;
  using pStereoData_t = PTR(StereoData_t);

  using Track_t = std::vector<StereoData_t>;

  using AudioCallback_t = std::function<StereoData_t(void)>;

    // Core
  using Recorder_t = PTR(Core::Recorder);
  using Driver_t   = PTR(Core::Driver);
  using Node_t     = PTR(Core::Node);
  using Sound_t    = PTR(Core::Sound);

    // Generator
  using GenBase_t = PTR(Generator::Base);
  using Sine_t    = PTR(Generator::Sine);
  using Square_t  = PTR(Generator::Square);
  using WAV_t     = PTR(Generator::WAV);

    // Modifier
  using ModBase_t  = PTR(Modifier::Base);
  using BandPass_t = PTR(Modifier::BandPass);
  using Envelope_t = PTR(Modifier::EnvelopeFollower);
  using Gain_t     = PTR(Modifier::Gain);
  using Filter_t   = PTR(Modifier::GenericFilter);
  using LowPass_t  = PTR(Modifier::LowPass);

    // Sounds
  using SoundsBase_t = PTR(Sounds::Base);
  using Vocoder_t    = PTR(Sounds::Vocoder);

    // Tools
  using Resampler_t = PTR(Tools::Resampler);

} // namespace AudioEngine

#endif  // __TYPES_HPP
