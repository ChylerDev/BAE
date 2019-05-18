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
#include <tuple>

// Public Macros                ////////////////////////////////////////////////

#define PTR(x) std::shared_ptr<x>

// Forward References           ////////////////////////////////////////////////\ 

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

    // StereoData_t[0] = left; StereoData_t[1] = right;
  using StereoData_t = std::tuple<float,float>;
  using pStereoData_t = PTR(StereoData_t);

  using Track_t = std::vector<StereoData_t>;

  using AudioCallback_t = std::function<StereoData_t(void)>;

    // Core
  using Recorder_t = Core::Recorder;
  using pRecorder_t = PTR(Recorder_t);
  using Driver_t = Core::Driver;
  using pDriver_t = PTR(Driver_t);
  using Node_t = Core::Node;
  using pNode_t = PTR(Node_t);
  using Sound_t = Core::Sound;
  using pSound_t = PTR(Sound_t);

    // Generator
  using GenBase_t = Generator::Base;
  using pGenBase_t = PTR(GenBase_t);
  using Sine_t = Generator::Sine;
  using pSine_t = PTR(Sine_t);
  using Square_t = Generator::Square;
  using pSquare_t = PTR(Square_t);
  using WAV_t = Generator::WAV;
  using pWAV_t = PTR(WAV_t);

    // Modifier
  using ModBase_t = Modifier::Base;
  using pModBase_t = PTR(ModBase_t);
  using BandPass_t = Modifier::BandPass;
  using pBandPass_t = PTR(BandPass_t);
  using Envelope_t = Modifier::EnvelopeFollower;
  using pEnvelope_t = PTR(Envelope_t);
  using Gain_t = Modifier::Gain;
  using pGain_t = PTR(Gain_t);
  using Filter_t = Modifier::GenericFilter;
  using pFilter_t = PTR(Filter_t);
  using LowPass_t = Modifier::LowPass;
  using pLowPass_t = PTR(LowPass_t);

    // Sounds
  using SoundsBase_t = Sounds::Base;
  using pSoundsBase_t = PTR(SoundsBase_t);
  using Vocoder_t = Sounds::Vocoder;
  using pVocoder_t = PTR(Vocoder_t);

    // Tools
  using Resampler_t = Tools::Resampler;
  using pResampler_t = PTR(Resampler_t);

} // namespace AudioEngine

#endif  // __TYPES_HPP
